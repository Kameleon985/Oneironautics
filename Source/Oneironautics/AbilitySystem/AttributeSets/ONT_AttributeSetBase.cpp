// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/ONT_AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UONT_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxMovemenentSpeedAttribute())
	{
		ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
		UCharacterMovementComponent* CharacterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

		if (CharacterMovement)
		{
			const float MaxSpeed = GetMaxMovemenentSpeed();

			CharacterMovement->MaxWalkSpeed = MaxSpeed;
		}
	}
}

void UONT_AttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UONT_AttributeSetBase, Health, OldHealth);
}

void UONT_AttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UONT_AttributeSetBase, MaxHealth, OldMaxHealth);
}

void UONT_AttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UONT_AttributeSetBase, Stamina, OldStamina);
}

void UONT_AttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UONT_AttributeSetBase, MaxStamina, OldMaxStamina);
}

void UONT_AttributeSetBase::OnRep_MaxMovemenentSpeed(const FGameplayAttributeData& OldMaxMovemenentSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UONT_AttributeSetBase, MaxMovemenentSpeed, OldMaxMovemenentSpeed);

}

void UONT_AttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UONT_AttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UONT_AttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UONT_AttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UONT_AttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UONT_AttributeSetBase, MaxMovemenentSpeed, COND_None, REPNOTIFY_Always);

}