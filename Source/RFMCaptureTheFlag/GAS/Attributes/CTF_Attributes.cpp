// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/CTF_Attributes.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UCTF_Attributes::UCTF_Attributes()
{
	// Default values
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitBaseWalkSpeed(600.0f);
	InitBaseJumpHeight(420.0f);
}

void UCTF_Attributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCTF_Attributes, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCTF_Attributes, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCTF_Attributes, BaseWalkSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCTF_Attributes, BaseJumpHeight, COND_None, REPNOTIFY_Always);
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
void UCTF_Attributes::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	AActor* OwningActor = GetOwningAbilitySystemComponent()->GetAvatarActor();
	if (!OwningActor) return;

	ACharacter* Character = Cast<ACharacter>(OwningActor);
	if (!Character) return;

	UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
	if (!MovementComp) return;

	
	if (Attribute == GetBaseWalkSpeedAttribute())
	{
		MovementComp->MaxWalkSpeed = NewValue;
	}
	
	else if (Attribute == GetBaseJumpHeightAttribute())
	{
		MovementComp->JumpZVelocity = NewValue;
	}
}
void UCTF_Attributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
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

void UCTF_Attributes::OnRep_BaseWalkSpeed(const FGameplayAttributeData& OldBaseWalkSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCTF_Attributes, BaseWalkSpeed, OldBaseWalkSpeed);
	
	AActor* OwningActor = GetOwningAbilitySystemComponent()->GetAvatarActor();
	if (OwningActor)
	{
		if (ACharacter* Character = Cast<ACharacter>(OwningActor))
		{
			if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
			{
				MovementComp->MaxWalkSpeed = GetBaseWalkSpeed();
			}
		}
	}
}

void UCTF_Attributes::OnRep_BaseJumpHeight(const FGameplayAttributeData& OldBaseJumpHeight)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCTF_Attributes, BaseJumpHeight, OldBaseJumpHeight);

	AActor* OwningActor = GetOwningAbilitySystemComponent()->GetAvatarActor();
	if (OwningActor)
	{
		if (ACharacter* Character = Cast<ACharacter>(OwningActor))
		{
			if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
			{
				MovementComp->JumpZVelocity = GetBaseJumpHeight();
			}
		}
	}
}