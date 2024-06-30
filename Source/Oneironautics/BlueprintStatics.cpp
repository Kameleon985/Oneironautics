// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintStatics.h"
#include "ONT_Types.h"

const UItemStaticData* UBlueprintStatics::GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass)
{
	if (IsValid(ItemDataClass))
	{
		return GetDefault<UItemStaticData>(ItemDataClass);
	}

	return nullptr;
}
