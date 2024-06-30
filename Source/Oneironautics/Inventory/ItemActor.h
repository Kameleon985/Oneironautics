// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ONT_Types.h"
#include "ItemActor.generated.h"

class UInventoryItemInstance;

UCLASS()
class ONEIRONAUTICS_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemActor();

	virtual void Tick(float DeltaTime) override;

	virtual void OnEquipped();
	virtual void OnUneqipped();
	virtual void OnDropped();

	bool ReplicateSubobjects(class UActorChannel* Channel,class FOutBunch* Bunch, FReplicationFlags* RepFlags);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void Init(UInventoryItemInstance* InInstance);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	UInventoryItemInstance* ItemInstance = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	TEnumAsByte<EItemState> ItemState = EItemState::None;

	UFUNCTION()
	void OnRep_ItemState();

	UPROPERTY()
	class USphereComponent* SphereComponent = nullptr;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;

};
