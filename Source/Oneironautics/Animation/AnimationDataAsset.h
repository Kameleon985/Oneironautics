// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ONT_AnimInstance.h"
#include "AnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ONEIRONAUTICS_API UAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FCharacterAnimationData CharacterAnimationData;

};
