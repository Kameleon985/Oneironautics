// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/ONT_AnimInstance.h"
#include "ONT_Types.generated.h"


UCLASS(BlueprintType,Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AItemActor> ItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachmentSocket = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeEquipped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterAnimationData CharaterAnimationData;
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None UMETA(DistaplyName = "None"),
	Equipped UMETA(DistaplyName = "Equipped"),
	Dropped UMETA(DistaplyName = "Dropped")
};