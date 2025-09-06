// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/CTF_Attributes.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h" 

UCTF_Attributes::UCTF_Attributes()
{
	// Default values
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

void UCTF_Attributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCTF_Attributes, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCTF_Attributes, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCTF_Attributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp health between 0 and MaxHealth.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		// Check if health has dropped to or below zero
		if (GetHealth() <= 0.0f)
		{
			// Broadcast the OnDeath delegate instead of calling a function directly.
			// This decouples the attribute set from the character class.
			OnCharacterDeath.Broadcast();
		}
		// Broadcast the health change event.
		OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth());
	}

}

void UCTF_Attributes::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCTF_Attributes, Health, OldHealth);
	OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth()); 
}

void UCTF_Attributes::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCTF_Attributes, MaxHealth, OldMaxHealth);
	OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth()); 
}