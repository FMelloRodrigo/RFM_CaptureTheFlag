// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTF_WeaponsBase.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SemiAuto,
	FullAuto
};

// Delegate to broadcast ammo changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedDelegate, int32, CurrentAmmo, int32, MaxAmmo);

UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_WeaponsBase : public AActor
{
	GENERATED_BODY()
	
public:	

	ACTF_WeaponsBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void StartFire();
	void StopFire();
	void ToggleFireMode();

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo() const { return MaxAmmo; }

	// Delegate that will be broadcasted when ammo changes
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoChangedDelegate OnAmmoChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class ACTF_ProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireRate; // Rounds per minute

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmo, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EFireMode CurrentFireMode;

	UFUNCTION()
	void OnRep_CurrentAmmo();

private:
	FTimerHandle TimerHandle_AutoFire;
	FTimerHandle TimerHandle_AmmoRegen;
	float TimeBetweenShots;
	bool bIsFiring;

	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();

	void StartAmmoRegen();
	void RegenerateAmmo();

};
