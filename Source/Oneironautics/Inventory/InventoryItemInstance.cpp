// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"
#include "../BlueprintStatics.h"
#include "ItemActor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

const UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UBlueprintStatics::GetItemStaticData(ItemStaticDataClass);
}

void UInventoryItemInstance::OnRep_Equipped()
{

}


void UInventoryItemInstance::OnEquipped(AActor* InOwner)
{
	//Propably not the best solution
	if (UWorld* World = GEngine->GetWorldFromContextObject(InOwner,EGetWorldErrorMode::ReturnNull))
	{
		const UItemStaticData* StaticData = GetItemStaticData();

		FTransform Transform;
		ItemActor = World->SpawnActorDeferred<AItemActor>(GetItemStaticData()->ItemActorClass, Transform, InOwner);
		ItemActor->Init(this);
		ItemActor->OnEquipped();
		ItemActor->FinishSpawning(Transform);
		
		ACharacter* Character = Cast<ACharacter>(InOwner);
		if (USkeletalMeshComponent* SkeletalMesh = Character ? Character->GetMesh() : nullptr)
		{
			ItemActor->AttachToActor(InOwner, FAttachmentTransformRules::SnapToTargetNotIncludingScale, StaticData->AttachmentSocket);
			bEquipped = true;
		}
	}
}

void UInventoryItemInstance::OnUnequipped()
{
	if (ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}

	bEquipped = false;
}

void UInventoryItemInstance::OnDropped()
{
	if (ItemActor)
	{
		ItemActor->OnDropped();
	}

	bEquipped = false;
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UInventoryItemInstance, bEquipped);
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
}