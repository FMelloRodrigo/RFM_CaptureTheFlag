// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CTF/Actors/CTF_Base.h"

#include "Components/BoxComponent.h"
#include "GameModes/CTF/GameMode/CTF_GameMode.h"
#include "GameModes/CTF/Actors/CTF_Flag.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACTF_Base::ACTF_Base()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and set up the collision component
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void ACTF_Base::BeginPlay()
{
	Super::BeginPlay();

	// Register the overlap event only on the server
	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACTF_Base::OnOverlapBegin);
	}
}

void ACTF_Base::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is a player pawn
	APawn* PlayerPawn = Cast<APawn>(OtherActor);
	if (PlayerPawn)
	{
		ACTF_PlayerState* PlayerState = PlayerPawn->GetPlayerState<ACTF_PlayerState>();
		if (PlayerState && PlayerState->GetTeam() == Team)
		{
			// Check if the player is carrying the flag
			ACTF_Flag* FlagActor = Cast<ACTF_Flag>(PlayerPawn->GetOwner());
			if (FlagActor)
			{
				// Get the Game Mode and call the score function
				ACTF_GameMode* GameMode = Cast<ACTF_GameMode>(UGameplayStatics::GetGameMode(this));
				if (GameMode)
				{
					GameMode->OnScore(PlayerPawn->GetController<APlayerController>());
				}
			}
		}
	}
}



