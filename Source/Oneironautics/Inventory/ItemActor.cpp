// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemActor.h"
#include "InventoryItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "../Components/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "InventoryList.h"
#include <AbilitySystemBlueprintLibrary.h>

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnSphereBeginOverlap);

	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemStaticDataClass))
		{
			ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->Init(ItemStaticDataClass);

			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

void AItemActor::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::Dropped:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	default:
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	}
}

void AItemActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = UInventoryComponent::EquipItemTag;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemTag, EventPayload);
	}
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::OnEquipped()
{
	ItemState = EItemState::Equipped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemActor::OnUneqipped()
{
	ItemState = EItemState::None;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemActor::OnDropped()
{
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ItemState = EItemState::Dropped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

bool AItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		

	return WroteSomething;
}


void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemInstance);
	DOREPLIFETIME(AItemActor, ItemState);
}

void AItemActor::Init(UInventoryItemInstance* InInstance)
{
	ItemInstance = InInstance;
}
