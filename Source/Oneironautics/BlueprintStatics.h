// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintStatics.generated.h"

/**
 * 
 */
UCLASS()
class ONEIRONAUTICS_API UBlueprintStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const class UItemStaticData* GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass);

};
