// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CTF/GameMode/CTF_GameMode.h"

#include "GameModes/CTF/States/CTF_GameState.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "GameModes/CTF/Actors/CTF_PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "GameModes/CTF/Actors/CTF_Flag.h"
#include "GameModes/CTF/Actors/CTF_Base.h"

ACTF_GameMode::ACTF_GameMode()
{
	//bStartPlayersAsSpectators = true;
	//bDelayedStart = true;
}

void ACTF_GameMode::PostLogin(APlayerController* NewPlayer)
{
	
	Super::PostLogin(NewPlayer);
	/*
	ACTF_GameState* CtfGameState = GetGameState<ACTF_GameState>();
	ACTF_PlayerState* CtfPlayerState = NewPlayer->GetPlayerState<ACTF_PlayerState>();
	
	if (CtfGameState && CtfPlayerState)
	{
		// Determine which team has fewer players
		if (CtfGameState->RedTeamPlayers.Num() <= CtfGameState->BlueTeamPlayers.Num())
		{
			// Assign to Red Team (first player goes here too)
			CtfPlayerState->SetTeam(ETeam::RedTeam);
			CtfGameState->AddPlayerToTeam(CtfPlayerState, ETeam::RedTeam);
		}
		else
		{
			// Assign to Blue Team
			CtfPlayerState->SetTeam(ETeam::BlueTeam);
			CtfGameState->AddPlayerToTeam(CtfPlayerState, ETeam::BlueTeam);
		}


		UE_LOG(LogTemp, Warning, TEXT("Player %s assigned to a team."), *CtfPlayerState->GetPlayerName());

		//StartMatch();

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get CTFGameState or CTFPlayerState in PostLogin."));
		GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("Failed to get CTFGameState or CTFPlayerState in PostLogin.")));
	}
	*/
}

void ACTF_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ACTF_GameState* CtfGameState = GetGameState<ACTF_GameState>();
	ACTF_PlayerState* CtfPlayerState = Exiting->GetPlayerState<ACTF_PlayerState>();
	if (CtfGameState && CtfPlayerState)
	{
		if (CtfGameState->RedTeamPlayers.Contains(CtfPlayerState))
		{
			CtfGameState->RedTeamPlayers.Remove(CtfPlayerState);
		}
		if (CtfGameState->BlueTeamPlayers.Contains(CtfPlayerState))
		{
			CtfGameState->BlueTeamPlayers.Remove(CtfPlayerState);
		}

	}
}

void ACTF_GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);	

}

AActor* ACTF_GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	
	ACTF_GameState* CtfGameState = GetGameState<ACTF_GameState>();
	ACTF_PlayerState* CtfPlayerState = Cast<ACTF_PlayerState>(Player->PlayerState);
	
	if (!CtfPlayerState || !CtfGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChoosePlayerStart_Implementation: PlayerState is not a valid ACTF_PlayerState. Cannot determine team."));
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	// Get all CTF_PlayerStart actors
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTF_PlayerStart::StaticClass(), PlayerStarts);
	
	if (CtfGameState->RedTeamPlayers.Num() <= CtfGameState->BlueTeamPlayers.Num())
	{
		// Assign to Red Team (first player goes here too)
		CtfPlayerState->SetTeam(ETeam::RedTeam);
		CtfGameState->AddPlayerToTeam(CtfPlayerState, ETeam::RedTeam);
		for (AActor* StartActor : PlayerStarts)
		{
			ACTF_PlayerStart* PlayerStart = Cast<ACTF_PlayerStart>(StartActor);

			if (PlayerStart && PlayerStart->Team == ETeam::RedTeam)
			{
				//UE_LOG(LogTemp, Log, TEXT("Found a valid spawn point for team: %s"), (CtfPlayerState->GetTeam() == ETeam::RedTeam) ? TEXT("Red Team") : TEXT("Blue Team"));
				GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("ASSINED TO RED TEAM")));
				return PlayerStart;
			}
		}

	}
	else
	{
		// Assign to Blue Team
		CtfPlayerState->SetTeam(ETeam::BlueTeam);
		CtfGameState->AddPlayerToTeam(CtfPlayerState, ETeam::BlueTeam);

		for (AActor* StartActor : PlayerStarts)
		{
			ACTF_PlayerStart* PlayerStart = Cast<ACTF_PlayerStart>(StartActor);

			if (PlayerStart && PlayerStart->Team == ETeam::BlueTeam)
			{
				//UE_LOG(LogTemp, Log, TEXT("Found a valid spawn point for team: %s"), (CtfPlayerState->GetTeam() == ETeam::RedTeam) ? TEXT("Red Team") : TEXT("Blue Team"));
				//GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("Current State: %s"), *UEnum::GetValueAsString(CtfPlayerState->GetTeam())));
				GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("ASSINED TO BLUE TEAM")));
				return PlayerStart;
			}
		}
	}
	/*
	// Find the correct PlayerStart for the player's team
	for (AActor* StartActor : PlayerStarts)
	{
		ACTF_PlayerStart* PlayerStart = Cast<ACTF_PlayerStart>(StartActor);
		
		if (PlayerStart && PlayerStart->Team == CtfPlayerState->GetTeam())
		{
			//UE_LOG(LogTemp, Log, TEXT("Found a valid spawn point for team: %s"), (CtfPlayerState->GetTeam() == ETeam::RedTeam) ? TEXT("Red Team") : TEXT("Blue Team"));
			//GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("Current State: %s"), *UEnum::GetValueAsString(CtfPlayerState->GetTeam())));
			return PlayerStart;
		}
	}

	// Fallback to the default behavior if no suitable player start is found
	//UE_LOG(LogTemp, Warning, TEXT("No suitable PlayerStart found for team: %s. Falling back to default spawn logic."), (CtfPlayerState->GetTeam() == ETeam::RedTeam) ? TEXT("Red Team") : TEXT("Blue Team"));
	
	*/
	GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("No suitable PlayerStart found for team. Falling back to default spawn logic")));
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ACTF_GameMode::HandleMatchHasStarted()
{
	//GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("Current State")));
	Super::HandleMatchHasStarted();
/*
	ACTF_GameState* CtfGameState = Cast<ACTF_GameState>(UGameplayStatics::GetGameState(this));
	if (CtfGameState)
	{

		for (auto LocalPState : CtfGameState->PlayerArray)
		{
			ACTF_PlayerState* LoopPState = Cast<ACTF_PlayerState>(LocalPState);

			if (LoopPState && LoopPState->GetTeam() == ETeam::None)
			{
				if (CtfGameState->RedTeamPlayers.Num() <= CtfGameState->BlueTeamPlayers.Num())
				{
					LoopPState->SetTeam(ETeam::RedTeam);
					CtfGameState->AddPlayerToTeam(LoopPState, ETeam::RedTeam);
				}
				else
				{
					LoopPState->SetTeam(ETeam::BlueTeam);
					CtfGameState->AddPlayerToTeam(LoopPState, ETeam::BlueTeam);
				}
			}
		}	
	}
	*/

	// Spawn the flag at the beginning of the game
	FindInitialFlag();
}

void ACTF_GameMode::FindInitialFlag()
{
	FlagActor = Cast< ACTF_Flag> (UGameplayStatics::GetActorOfClass(GetWorld(), ACTF_Flag::StaticClass()));
}

void ACTF_GameMode::OnFlagPickedUp(APawn* PlayerPawn, ACTF_Flag* Flag)
{
	if (FlagActor && FlagActor == Flag)
	{
		// Hide the flag and attach it to the player
		FlagActor->SetActorHiddenInGame(true);
		FlagActor->SetOwner(PlayerPawn);
	}
}

void ACTF_GameMode::OnScore(APlayerController* Scorer)
{
	if (!Scorer) return;

	ACTF_PlayerState* ScorerPS = Scorer->GetPlayerState<ACTF_PlayerState>();
	if (ScorerPS)
	{
		ACTF_GameState* PGameState = GetGameState<ACTF_GameState>();
		if (PGameState)
		{
			PGameState->AddScoreToTeam(ScorerPS->GetTeam(), 1);
			ResetFlag();

			if (PGameState->RedTeamScore >= 3 || PGameState->BlueTeamScore >= 3)
			{
				ResetGame();
			}
		}
	}
}

void ACTF_GameMode::ResetFlag()
{
	// Reset the flag to the center of the map
	if (FlagActor)
	{
		FlagActor->SetActorHiddenInGame(false);
		FlagActor->SetOwner(nullptr);

		// Find all base actors to determine the center of the map
		TArray<AActor*> Bases;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTF_Base::StaticClass(), Bases);

		FVector CenterOfMap = FVector::ZeroVector;
		if (Bases.Num() > 0)
		{
			for (AActor* Base : Bases)
			{
				CenterOfMap += Base->GetActorLocation();
			}
			CenterOfMap /= Bases.Num();
		}

		FlagActor->SetActorLocation(CenterOfMap);
	}
}

void ACTF_GameMode::ResetGame()
{
	// Reset scores to 0
	ACTF_GameState* PGameState = GetGameState<ACTF_GameState>();
	if (PGameState)
	{
		PGameState->RedTeamScore = 0;
		PGameState->BlueTeamScore = 0;
	}

	// Respawn all players and reset flag
	TArray<AActor*> PlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), PlayerControllers);

	for (AActor* ControllerActor : PlayerControllers)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(ControllerActor))
		{
			RestartPlayer(PlayerController);
		}
	}
	ResetFlag();
}

void ACTF_GameMode::PlayerKilled(AController* Killer, AController* Victim)
{
	if (Killer && Victim)
	{
		// Restart the killed player after a delay
		FTimerHandle RestartTimerHandle;
		FTimerDelegate RestartDelegate;
		RestartDelegate.BindUFunction(this, FName("RestartPlayer"), Victim);
		GetWorldTimerManager().SetTimer(RestartTimerHandle, RestartDelegate, 5.0f, false);
	}
}