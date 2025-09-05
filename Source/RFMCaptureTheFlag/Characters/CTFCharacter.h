// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "InputActionValue.h" 
#include "GameModes/CTF/Interfaces/ICTF_Teams.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "CTFCharacter.generated.h"

class UAbilitySystemComponent;
class UCTF_Attributes;
class UGameplayEffect;
class ACTF_WeaponsBase;
class UInputMappingContext; 
class UInputAction;     

// Delegate for when the character equips a new weapon
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedDelegate, ACTF_WeaponsBase*, NewWeapon);



UCLASS()
class RFMCAPTURETHEFLAG_API ACTFCharacter : public ACharacter, public IAbilitySystemInterface, public IICTF_Teams
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACTFCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(ACTF_WeaponsBase* WeaponToEquip);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	ACTF_WeaponsBase* GetEquippedWeapon() const { return EquippedWeapon; }

	// Delegate that broadcasts when a weapon is equipped.
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponEquippedDelegate OnWeaponEquipped;

	// TEAMS
	UFUNCTION(BlueprintCallable,Category = "Teams")
	void OnTeamsInit(ETeam InitTeam);
	
	void OnTeamsChanged_Implementation(ETeam PlayerTeam) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* FirstPersonMesh;


	void UpdateCharacterTeamColor(ETeam NewTeam);

	UPROPERTY()
	UMaterialInstanceDynamic* TeamMaterialInstance;

	UPROPERTY()
	UMaterialInstanceDynamic* TeamMaterialInstance2;

protected:
	virtual void BeginPlay() override;
	

	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	
	void StartFire(const FInputActionValue& Value);
	void StopFire(const FInputActionValue& Value);
	void ToggleFireMode(const FInputActionValue& Value);

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCameraComponent;

	// Components


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UCTF_Attributes* HealthAttributeSet;

	// The weapon the character is currently holding
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ACTF_WeaponsBase* EquippedWeapon;

	// The weapon the character is currently holding
	UFUNCTION()
	void OnRep_EquippedWeapon();

	// The class of weapon to spawn and equip at the start
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ACTF_WeaponsBase> DefaultWeaponClass;

	// Default abilities to grant to the character on startup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	// Default gameplay effects to apply on startup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	// ENHANCED INPUT: Properties for Input assets
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

	void GiveDefaultAbilitiesAndEffects();

private:
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(ACTF_WeaponsBase* WeaponToEquip);

};
