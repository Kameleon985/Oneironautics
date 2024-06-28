// Fill out your copyright notice in the Description page of Project Settings.


#include "../OneironauticsCharacter.h"
#include "../Animation/AnimationDataAsset.h"
#include "Animation/ONT_AnimInstance.h"

UBlendSpace* UONT_AnimInstance::GetLocomotionBlendspace() const
{
	if (AOneironauticsCharacter* ONTCharacter = Cast<AOneironauticsCharacter>(GetOwningActor()))
	{
		FCharacterData Data = ONTCharacter->GetCharacterData();

		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.MovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.MovementBlendspace : nullptr;
}

UAnimSequenceBase* UONT_AnimInstance::GetIdleAnimation() const
{
	if (AOneironauticsCharacter* ONTCharacter = Cast<AOneironauticsCharacter>(GetOwningActor()))
	{
		FCharacterData Data = ONTCharacter->GetCharacterData();

		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.IdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset : nullptr;
}

UBlendSpace* UONT_AnimInstance::GetLocomotionCrouchBlendspace() const
{
	if (AOneironauticsCharacter* ONTCharacter = Cast<AOneironauticsCharacter>(GetOwningActor()))
	{
		FCharacterData Data = ONTCharacter->GetCharacterData();

		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.CrouchMovementBlendspace;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchMovementBlendspace : nullptr;
}

UAnimSequenceBase* UONT_AnimInstance::GetIdleCrouchAnimation() const
{
	if (AOneironauticsCharacter* ONTCharacter = Cast<AOneironauticsCharacter>(GetOwningActor()))
	{
		FCharacterData Data = ONTCharacter->GetCharacterData();

		if (Data.CharacterAnimationData)
		{
			return Data.CharacterAnimationData->CharacterAnimationData.CrouchIdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;
}
