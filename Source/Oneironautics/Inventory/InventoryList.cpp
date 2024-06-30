// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryList.h"
#include "Inventory/InventoryItemInstance.h"

void FInventoryList::AddItem(TSubclassOf<UItemStaticData> InItemstaticDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemIstance = NewObject<UInventoryItemInstance>();
	Item.ItemIstance->Init(InItemstaticDataClass);
	MarkItemDirty(Item);
}

void FInventoryList::AddItem(UInventoryItemInstance* ItemInstance)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemIstance = ItemInstance;
	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(TSubclassOf<UItemStaticData> InItemstaticDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem Item = *ItemIter;
		if (Item.ItemIstance && Item.ItemIstance->GetItemStaticData()->IsA(InItemstaticDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}
