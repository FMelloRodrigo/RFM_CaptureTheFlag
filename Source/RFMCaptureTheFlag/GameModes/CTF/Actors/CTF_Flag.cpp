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
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and set up the collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// Create and set up the flag mesh
	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	FlagMesh->SetupAttachment(RootComponent);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsHeld = false;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACTF_Flag::BeginPlay()
{
	Super::BeginPlay();

	// Register the overlap event only on the server
	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACTF_Flag::OnOverlapBegin);
	}
}

void ACTF_Flag::OnFlagDropped()
{
	// This function should be called by the player's pawn when it dies.
	// We make it an RPC so it can be called from the client if needed, but the main logic should be server-side.
	if (HasAuthority())
	{
		SetActorHiddenInGame(false);
		SetOwner(nullptr);
		bIsHeld = false;
	}
}

void ACTF_Flag::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is a player pawn
	APawn* PlayerPawn = Cast<APawn>(OtherActor);
	if (PlayerPawn && !bIsHeld)
	{
		ACTF_PlayerState* PlayerState = PlayerPawn->GetPlayerState<ACTF_PlayerState>();
		if (PlayerState)
		{
			// Get the Game Mode and call the flag pickup function
			ACTF_GameMode* GameMode = Cast<ACTF_GameMode>(UGameplayStatics::GetGameMode(this));
			if (GameMode)
			{
				GameMode->OnFlagPickedUp(PlayerPawn, this);
				bIsHeld = true;
			}
		}
	}
}

void ACTF_Flag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTF_Flag, bIsHeld);
}




