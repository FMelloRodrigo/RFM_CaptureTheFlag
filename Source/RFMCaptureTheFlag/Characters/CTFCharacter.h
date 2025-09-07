// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "InputActionValue.h" 
#include "GameplayTagContainer.h"
#include "GameModes/CTF/Interfaces/ICTF_Teams.h"
#include "GAS/AbilitiesEnums.h"
#include "CTFCharacter.generated.h"

class UAbilitySystemComponent;
class UCTF_Attributes;
class UGameplayEffect;
class ACTF_WeaponsBase;
class UInputMappingContext; 
class UInputAction;     

// Delegate for when the character equips a new weapon
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedDelegate, ACTF_WeaponsBase*, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayEffectAppliedDelegate, FGameplayTagContainer, EffectTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayEffectRemovedDelegate, FGameplayTagContainer, EffectTags);



UCLASS()
class RFMCAPTURETHEFLAG_API ACTFCharacter : public ACharacter, public IAbilitySystemInterface, public IICTF_Teams
{
	GENERATED_BODY()

public:
	
	ACTFCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* FirstPersonMesh;

	UFUNCTION(Client, Reliable)
	void CreatePlayerHUD();

// GAS
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetWalkSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetJumpHeight() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

// Weapons 
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(ACTF_WeaponsBase* WeaponToEquip);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	ACTF_WeaponsBase* GetEquippedWeapon() const { return EquippedWeapon; }

	
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponEquippedDelegate OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Effects")
	FOnGameplayEffectAppliedDelegate GEApplied;

	UPROPERTY(BlueprintAssignable, Category = "Effects")
	FOnGameplayEffectRemovedDelegate GERemoved;


#pragma region TeamColors


	// Calback interface called when team are set, not used currently but can be used for some visuals or anything later
	UFUNCTION(BlueprintCallable, Category = "Teams")
	void OnTeamsInit(ETeam InitTeam);

	void OnTeamsChanged_Implementation(ETeam PlayerTeam) override;

	void UpdateCharacterTeamColor(ETeam NewTeam);

	UPROPERTY()
	UMaterialInstanceDynamic* TeamMaterialInstance;

	UPROPERTY()
	UMaterialInstanceDynamic* TeamMaterialInstance2;

#pragma endregion TeamColors

protected:
	
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void ToggleFireMode(const FInputActionValue& Value);


	virtual void OnRep_PlayerState() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCameraComponent;


# pragma region Death Events
	
	UFUNCTION()
	void Die();


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnDeath();


	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead = false;


	UFUNCTION()
	void OnRep_IsDead();


	virtual void PawnClientRestart() override;

	

# pragma endregion Death Events


// GAS 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UCTF_Attributes* HealthAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	void GiveDefaultAbilitiesAndEffects();

	UFUNCTION(Client, Unreliable)
	void GEAppliedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION(Client, Unreliable)
	void GERemovedCallback(const FActiveGameplayEffect& Effect);

// Weapons 	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ACTF_WeaponsBase* EquippedWeapon;

	UFUNCTION()
	void OnRep_EquippedWeapon();


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ACTF_WeaponsBase> DefaultWeaponClass;



#pragma region InputActions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ToggleFireModeAction;

#pragma endregion InputActions


private:

	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(ACTF_WeaponsBase* WeaponToEquip);

	void InputAbilityPressed(FGameplayTag InputTag);
	void InputAbilityReleased(FGameplayTag InputTag);



};
