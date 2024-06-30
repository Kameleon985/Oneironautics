// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Inventory/ItemActor.h"
#include "../Inventory/InventoryItemInstance.h"
#include "../Inventory/InventoryList.h"
#include "GameplayTagsManager.h"

FGameplayTag UInventoryComponent::EquipItemTag;
FGameplayTag UInventoryComponent::DropItemTag;
FGameplayTag UInventoryComponent::EquipNextTag;
FGameplayTag UInventoryComponent::UnequipTag;


static TAutoConsoleVariable<int32> CVarDebugShowInventory(
	TEXT("ShowDebugInventory"),
	0,
	TEXT("Draw debug info about inventory")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat
);

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	static bool bHandleAddingTags = false;
	if (!bHandleAddingTags)
	{
		bHandleAddingTags = true;
		UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UInventoryComponent::AddInventoryTags);
	}

}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	UInventoryComponent::EquipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItemActor"), TEXT("Equip item from item actor event"));
	UInventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.DropItem"), TEXT("Drop equipped item"));
	UInventoryComponent::EquipNextTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipNext"), TEXT("Try equip next item"));
	UInventoryComponent::UnequipTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.Unequip"), TEXT("Unequip current item"));

	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (auto ItemClass : DefaultItems)
		{
			InventoryList.AddItem(ItemClass);
		}
	}

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::DropItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipNextTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::UnequipTag).AddUObject(this, &UInventoryComponent::GameplayEventCallback);
	}
}

void UInventoryComponent::HandleGameplayEventInternal(FGameplayEventData Payload)
{
	ENetRole NetRole = GetOwnerRole();
	if (NetRole == ROLE_Authority)
	{
		FGameplayTag EventTag = Payload.EventTag;

		if (EventTag == UInventoryComponent::EquipItemTag)
		{
			if (const UInventoryItemInstance* ItemInstance = Cast<UInventoryItemInstance>(Payload.OptionalObject))
			{
				AddItemInstance(const_cast<UInventoryItemInstance*>(ItemInstance));

				if (Payload.Instigator)
				{
					const AActor* Instigator = Payload.Instigator;
					const_cast<AActor*>(Instigator)->Destroy();
				}
			}
		}
		else if (EventTag == UInventoryComponent::EquipNextTag)
		{
			EquipNext();
		}
		else if (EventTag == UInventoryComponent::DropItemTag)
		{
			DropItem();
		}
		else if (EventTag == UInventoryComponent::UnequipTag)
		{
			UnequipItem();
		}
	}
}

void UInventoryComponent::ServerHandleGameplayEvent_Implementation(FGameplayEventData Payload)
{
	HandleGameplayEventInternal(Payload);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bShowDebug = CVarDebugShowInventory.GetValueOnGameThread() != 0;

	if (bShowDebug)
	{
		for (FInventoryListItem& Item : InventoryList.GetItemsRef())
		{
			UInventoryItemInstance* ItemInstance = Item.ItemIstance;
			const UItemStaticData* ItemStaticData = ItemInstance->GetItemStaticData();

			if (IsValid(ItemInstance) && IsValid(ItemStaticData))
			{
				GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, FString::Printf(TEXT("Item: %s"), *ItemStaticData->Name.ToString()));
			}
		}
	}
}

void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> InItemstaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InItemstaticDataClass);
	}
}

void UInventoryComponent::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(InItemInstance);
	}
}

void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> InItemstaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(InItemstaticDataClass);
	}
}

void UInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> InItemstaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (auto Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemIstance->ItemStaticDataClass == InItemstaticDataClass)
			{
				Item.ItemIstance->OnEquipped(GetOwner());

				CurrentItem = Item.ItemIstance;

				break;
			}
		}
	}
}

void UInventoryComponent::EquipItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		for (auto Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemIstance == InItemInstance)
			{
				Item.ItemIstance->OnEquipped(GetOwner());
				CurrentItem = Item.ItemIstance;
				break;
			}
		}
	}

}

void UInventoryComponent::UnequipItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnUnequipped();
			CurrentItem = nullptr;
		}
	}
}

void UInventoryComponent::DropItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItem))
		{
			CurrentItem->OnDropped();
			RemoveItem(CurrentItem->ItemStaticDataClass);
			CurrentItem = nullptr;
		}
	}
}

void UInventoryComponent::EquipNext()
{
	if (GetOwner()->HasAuthority())
	{
		TArray<FInventoryListItem>& Items = InventoryList.GetItemsRef();

		const bool bNoItems = Items.Num() == 0;
		const bool bOneandEquipped = Items.Num() == 1 && CurrentItem;
		if (bNoItems || bOneandEquipped) return;

		UInventoryItemInstance* TargetItem = CurrentItem;

		for (auto Item : Items)
		{
			if (Item.ItemIstance->GetItemStaticData()->bCanBeEquipped)
			{
				if (Item.ItemIstance != CurrentItem)
				{
					TargetItem = Item.ItemIstance;
					break;
				}
			}
		}

		if (CurrentItem)
		{
			if (TargetItem == CurrentItem)
			{
				return;
			}
			UnequipItem();
		}

		EquipItemInstance(TargetItem);

	}
}

void UInventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEventInternal(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}
}




bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemIstance;

		if (IsValid(ItemInstance))
		{
			WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, CurrentItem);
}

UInventoryItemInstance* UInventoryComponent::GetEquippedItem() const
{
	return CurrentItem;
}
