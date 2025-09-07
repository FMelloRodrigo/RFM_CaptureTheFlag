// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_CTFCharacter_Shoot.h"
#include "Characters/CTFCharacter.h"
#include "Actors/CTF_WeaponsBase.h"

UGA_CTFCharacter_Shoot::UGA_CTFCharacter_Shoot()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Shoot")));
    ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("InputTag.Shoot")));
}

bool UGA_CTFCharacter_Shoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    ACTFCharacter* Character = Cast<ACTFCharacter>(ActorInfo->AvatarActor.Get());
    return Character && Character->GetEquippedWeapon();
}

void UGA_CTFCharacter_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        if (ACTFCharacter* Character = Cast<ACTFCharacter>(ActorInfo->AvatarActor.Get()))
        {
            LocalEquippedWeapon = Character->GetEquippedWeapon();
            if (LocalEquippedWeapon)
            {               
                LocalEquippedWeapon->StartFire();
                CommitAbility(Handle, ActorInfo, ActivationInfo);
            }
        }
    }

}

void UGA_CTFCharacter_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (LocalEquippedWeapon)
    {
        LocalEquippedWeapon->StopFire();
        LocalEquippedWeapon = nullptr;       
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

