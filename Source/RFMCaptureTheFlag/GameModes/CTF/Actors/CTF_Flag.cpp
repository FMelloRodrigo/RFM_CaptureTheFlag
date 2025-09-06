// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CTF/Actors/CTF_Flag.h"


#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameModes/CTF/GameMode/CTF_GameMode.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACTF_Flag::ACTF_Flag()
{
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	
	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	FlagMesh->SetupAttachment(RootComponent);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsHeld = false;
	
}

void ACTF_Flag::BeginPlay()
{
	Super::BeginPlay();

	
	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACTF_Flag::OnOverlapBegin);
	}
}

void ACTF_Flag::OnFlagDropped()
{
	if (HasAuthority())
	{
		SetActorHiddenInGame(false);
		SetOwner(nullptr);
		bIsHeld = false;
	}
}

void ACTF_Flag::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	APawn* PlayerPawn = Cast<APawn>(OtherActor);
	if (PlayerPawn && !bIsHeld)
	{
		ACTF_PlayerState* PlayerState = PlayerPawn->GetPlayerState<ACTF_PlayerState>();
		if (PlayerState)
		{
			ACTF_GameMode* GameMode = Cast<ACTF_GameMode>(UGameplayStatics::GetGameMode(this));
			if (GameMode && !GameMode->IsPlayerRespawning(PlayerPawn->GetController()))
			{
				
				GameMode->OnFlagPickedUp(PlayerPawn, this);
				FlagPickedEvents();
			}
		}
	}
}

void ACTF_Flag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTF_Flag, bIsHeld);
}

void ACTF_Flag::FlagPickedEvents()
{
	bIsHeld = true;
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACTF_Flag::EnableFlagCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}




