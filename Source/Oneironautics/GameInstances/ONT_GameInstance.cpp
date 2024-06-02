// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/ONT_GameInstance.h"
#include "AbilitySystemGlobals.h"

void UONT_GameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
