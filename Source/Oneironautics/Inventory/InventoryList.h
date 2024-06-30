// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ONT_Types.h"
#include "Engine/NetSerialization.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryList.generated.h"

class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryItemInstance* ItemIstance = nullptr;
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(Items, DeltaParams, *this);
	}

	void AddItem(TSubclassOf<UItemStaticData> InItemstaticDataClass);
	void AddItem(UInventoryItemInstance* ItemInstance);

	void RemoveItem(TSubclassOf<UItemStaticData> InItemstaticDataClass);

	TArray<FInventoryListItem>& GetItemsRef() { return Items; }

protected:
	UPROPERTY()
	TArray<FInventoryListItem> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum {WitNetDeltaSerializer = true};
};