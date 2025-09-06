// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CTFCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameMode.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/Attributes/CTF_Attributes.h"

#include "Actors/CTF_WeaponsBase.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "GameModes/CTF/Interfaces/ICTF_PlayerController.h"
#include "GameModes/CTF/Interfaces/ICTF_GameMode.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


ACTFCharacter::ACTFCharacter()
{

	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = true;
	bReplicates = true;

	GetMesh()->SetIsReplicated(true);
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(false);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetIsReplicated(true);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetOwnerNoSee(false);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HealthAttributeSet = CreateDefaultSubobject<UCTF_Attributes >(TEXT("HealthAttributeSet"));

}

void ACTFCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (HasAuthority())
	{
		if (DefaultWeaponClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			ACTF_WeaponsBase* StartingWeapon = GetWorld()->SpawnActor<ACTF_WeaponsBase>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			if (StartingWeapon)
			{
				EquipWeapon(StartingWeapon);
			}
		}
	}
	if (const UCTF_Attributes* LHealthAtributeSet = AbilitySystemComponent->GetSet<UCTF_Attributes>())
	{
		UCTF_Attributes* Attributes = const_cast<UCTF_Attributes*>(LHealthAtributeSet);
		Attributes->OnCharacterDeath.AddDynamic(this, &ACTFCharacter::Die);
		
	}
	
}

# pragma region Death Events

void ACTFCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// On the client, initialize the ASC from the replicated PlayerState
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	

}


void ACTFCharacter::Die()
{
	
	if (bIsDead)
	{
		return;
	}

	if (HasAuthority())
	{
		bIsDead = true;
		Server_OnDeath();
	}
}

void ACTFCharacter::Server_OnDeath_Implementation()
{
	
	IICTF_GameMode* Interface = Cast<IICTF_GameMode>(GetWorld()->GetAuthGameMode());
	AGameMode* GM = Cast<AGameMode>(GetWorld()->GetAuthGameMode());
	if (Interface && GM)
	{
		Interface->Execute_CTF_OnPlayerDeath(GM, GetController());
	}

	OnRep_IsDead();
}

bool ACTFCharacter::Server_OnDeath_Validate()
{
	return true;
}

void ACTFCharacter::OnRep_IsDead()
{
	// This function is called on both server and client
	if (bIsDead)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
		GetMesh()->SetSimulatePhysics(true);

		FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FirstPersonMesh->SetCollisionResponseToAllChannels(ECR_Block);
		FirstPersonMesh->SetSimulatePhysics(true);

		GetCharacterMovement()->DisableMovement();

		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->CancelAllAbilities();
		}

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			DisableInput(PC);
		}
	}
	else // Handle respawn
	{
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

		GetMesh()->SetHiddenInGame(false);
		FirstPersonMesh->SetHiddenInGame(false);

		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			EnableInput(PC);
		}
	}
}

void ACTFCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// When a character is restarted, reset the death state
	bIsDead = false;
	OnRep_IsDead();
}

# pragma endregion Death Events



void ACTFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ACTFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACTFCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACTFCharacter::Look);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACTFCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ACTFCharacter::StopFire);

		EnhancedInputComponent->BindAction(ToggleFireModeAction, ETriggerEvent::Started, this, &ACTFCharacter::ToggleFireMode);
	}
}


void ACTFCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}


void ACTFCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void ACTFCharacter::StartFire(const FInputActionValue& Value)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartFire();
	}
}


void ACTFCharacter::StopFire(const FInputActionValue& Value)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StopFire();
	}
}


void ACTFCharacter::ToggleFireMode(const FInputActionValue& Value)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->ToggleFireMode();
	}
}

void ACTFCharacter::OnRep_EquippedWeapon()
{
	OnWeaponEquipped.Broadcast(EquippedWeapon);
}

void ACTFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACTFCharacter, EquippedWeapon);
	DOREPLIFETIME(ACTFCharacter, bIsDead);
}

void ACTFCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GiveDefaultAbilitiesAndEffects();
	}

	CreatePlayerHUD();
}

void ACTFCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

UAbilitySystemComponent* ACTFCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ACTFCharacter::GetHealth() const
{
	if (HealthAttributeSet)
	{
		return HealthAttributeSet->GetHealth();
	}
	return 0.0f;
}

void ACTFCharacter::EquipWeapon(ACTF_WeaponsBase* WeaponToEquip)
{
	if (HasAuthority())
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquippedWeapon = WeaponToEquip;
		OnRep_EquippedWeapon(); // Manually call RepNotify for the server
		if (EquippedWeapon)
		{
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			EquippedWeapon->FPWeaponMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
			EquippedWeapon->TPWeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
		}
	}
	else
	{
		Server_EquipWeapon(WeaponToEquip);
	}
}

void ACTFCharacter::GiveDefaultAbilitiesAndEffects()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, INDEX_NONE));
		}

		for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
		{
			FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
			ContextHandle.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, ContextHandle);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void ACTFCharacter::Server_EquipWeapon_Implementation(ACTF_WeaponsBase* WeaponToEquip)
{
	EquipWeapon(WeaponToEquip);
}


void ACTFCharacter::OnTeamsInit(ETeam InitTeam)
{
}

void ACTFCharacter::OnTeamsChanged_Implementation(ETeam PlayerTeam)
{
	OnTeamsInit(PlayerTeam);
}

void ACTFCharacter::CreatePlayerHUD_Implementation()
{

	// Create Player HUD, calls interface on Player Controller
	if (IsLocallyControlled())
	{
		IICTF_PlayerController* Interface = Cast<IICTF_PlayerController>(GetController());
		if (Interface)
		{
			Interface->Execute_CTF_InitPlayerInput(GetController());
		}
	}



}

// Team Colors, should be on it's on actor components, but I didn't have time
void ACTFCharacter::UpdateCharacterTeamColor(ETeam NewTeam)
{
	
			if (!TeamMaterialInstance)
			{
				TeamMaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
				GetMesh()->SetMaterial(0, TeamMaterialInstance);
				FirstPersonMesh->SetMaterial(0, TeamMaterialInstance);
			}
			if (!TeamMaterialInstance2)
			{
				TeamMaterialInstance2 = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
				GetMesh()->SetMaterial(1, TeamMaterialInstance2);
				FirstPersonMesh->SetMaterial(1, TeamMaterialInstance2);
			}

			FLinearColor TeamColor;
			switch (NewTeam)
			{
			case ETeam::RedTeam:
				TeamColor = FLinearColor::Red;
				break;
			case ETeam::BlueTeam:
				TeamColor = FLinearColor::Blue;
				break;
			default:
				TeamColor = FLinearColor::White;
				break;
			}

			if (TeamMaterialInstance)
			{
				TeamMaterialInstance->SetVectorParameterValue(FName("Paint Tint"), TeamColor);
				
			}

			if (TeamMaterialInstance2)
			{
				TeamMaterialInstance2->SetVectorParameterValue(FName("Paint Tint"), TeamColor);
				
			}
}

//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Team Color Updated: %s"), *UEnum::GetValueAsString(NewTeam)));