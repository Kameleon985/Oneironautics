// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ONT_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ONEIRONAUTICS_API UONT_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
};
