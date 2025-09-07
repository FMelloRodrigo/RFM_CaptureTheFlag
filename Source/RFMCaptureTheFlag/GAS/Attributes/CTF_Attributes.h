// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CTF_Attributes.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)




// Delegate to broadcast health changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeathDelegate);


UCLASS()
class RFMCAPTURETHEFLAG_API UCTF_Attributes : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCTF_Attributes();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCTF_Attributes, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCTF_Attributes, MaxHealth);


	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_BaseWalkSpeed)
	FGameplayAttributeData BaseWalkSpeed;
	ATTRIBUTE_ACCESSORS(UCTF_Attributes, BaseWalkSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_BaseJumpHeight)
	FGameplayAttributeData BaseJumpHeight;
	ATTRIBUTE_ACCESSORS(UCTF_Attributes, BaseJumpHeight);


	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnHealthChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnCharacterDeathDelegate OnCharacterDeath;

protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_BaseWalkSpeed(const FGameplayAttributeData& OldBaseWalkSpeed);

	UFUNCTION()
	virtual void OnRep_BaseJumpHeight(const FGameplayAttributeData& OldBaseJumpHeight);

	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
};
