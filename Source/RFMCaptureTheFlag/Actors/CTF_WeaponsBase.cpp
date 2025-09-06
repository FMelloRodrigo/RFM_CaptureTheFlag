// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_WeaponsBase.h"
#include "CTF_ProjectileBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACTF_WeaponsBase::ACTF_WeaponsBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	FPWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPWeaponMesh"));
	TPWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPWeaponMesh"));
	
	FPWeaponMesh->SetupAttachment(SceneComp);
	FPWeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FPWeaponMesh->SetIsReplicated(true);

	TPWeaponMesh->SetupAttachment(SceneComp);
	TPWeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	TPWeaponMesh->SetIsReplicated(true);

	TPWeaponMesh->SetOnlyOwnerSee(false);
	TPWeaponMesh->SetOwnerNoSee(true);
	FPWeaponMesh->SetOnlyOwnerSee(true);
	FPWeaponMesh->SetOwnerNoSee(false);

	// Default Values for Weapon
	FireRate = 100.f;
	MaxAmmo = 30;
	CurrentFireMode = EFireMode::FullAuto;
	bIsFiring = false;

	if(HasAuthority())
	{
		OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
	}

}

void ACTF_WeaponsBase::BeginPlay()
{

	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
	TimeBetweenShots = 60.f / FireRate;
}

void ACTF_WeaponsBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACTF_WeaponsBase, CurrentAmmo);
	DOREPLIFETIME(ACTF_WeaponsBase, CurrentFireMode);
}

void ACTF_WeaponsBase::OnRep_CurrentAmmo()
{
	// This will be called on clients when CurrentAmmo changes
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
}

void ACTF_WeaponsBase::StartFire()
{
	if (!bIsFiring)
	{
		bIsFiring = true;
		if (CurrentFireMode == EFireMode::FullAuto)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &ACTF_WeaponsBase::Fire, TimeBetweenShots, true, 0.0f);
		}
		else
		{
			Fire();
		}
	}
}

void ACTF_WeaponsBase::StopFire()
{
	if (bIsFiring)
	{
		bIsFiring = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_AutoFire);
	}
}

void ACTF_WeaponsBase::ToggleFireMode()
{
	if (CurrentFireMode == EFireMode::FullAuto)
	{
		CurrentFireMode = EFireMode::SemiAuto;
	}
	else
	{
		CurrentFireMode = EFireMode::FullAuto;
	}
}

void ACTF_WeaponsBase::Fire()
{
	if (HasAuthority())
	{
		
		HandleFireOnServer();
	}
	else
	{
		
		Server_Fire();
	}
}

bool ACTF_WeaponsBase::Server_Fire_Validate()
{
	return true;
}

void ACTF_WeaponsBase::Server_Fire_Implementation()
{
	HandleFireOnServer();
}

void ACTF_WeaponsBase::StartAmmoRegen()
{
	RegenerateAmmo();
}

void ACTF_WeaponsBase::RegenerateAmmo()
{
	if (HasAuthority())
	{
		CurrentAmmo = MaxAmmo;
		OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo); // Broadcast for server player
	}
}

void ACTF_WeaponsBase::HandleFireOnServer()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;

		if (ProjectileClass)
		{
			const FVector MuzzleLocation = FPWeaponMesh->GetSocketLocation("Muzzle");
			const FRotator MuzzleRotation = FPWeaponMesh->GetSocketRotation("Muzzle");

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = Cast<APawn>(GetOwner());

			GetWorld()->SpawnActor<ACTF_ProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		}

		// Local feedback for the server player (clients get OnRep)
		OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
	}
	else
	{
		StopFire();
		StartAmmoRegen();
	}

}


