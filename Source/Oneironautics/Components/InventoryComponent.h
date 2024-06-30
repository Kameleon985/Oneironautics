// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Inventory/InventoryList.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONEIRONAUTICS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UItemStaticData> InItemstaticDataClass);

	UFUNCTION(BlueprintCallable)
	void AddItemInstance(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UItemStaticData> InItemstaticDataClass);

	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UItemStaticData> InItemstaticDataClass);

	UFUNCTION(BlueprintCallable)
	void EquipItemInstance(UInventoryItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintCallable)
	void EquipNext();

	virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	virtual void InitializeComponent() override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryItemInstance* GetEquippedItem() const;

	static FGameplayTag EquipItemTag;
	static FGameplayTag DropItemTag;
	static FGameplayTag EquipNextTag;
	static FGameplayTag UnequipTag;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void AddInventoryTags();

	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticData>> DefaultItems;

	UPROPERTY(Replicated)
	UInventoryItemInstance* CurrentItem = nullptr;

	void HandleGameplayEventInternal(FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(FGameplayEventData Payload);
};
