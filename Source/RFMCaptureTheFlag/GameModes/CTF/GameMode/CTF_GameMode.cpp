// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CTF/GameMode/CTF_GameMode.h"

#include "GameModes/CTF/States/CTF_GameState.h"
#include "GameModes/CTF/Actors/CTF_PlayerStart.h"
#include "Controllers/CTF_PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "GameModes/CTF/Actors/CTF_Flag.h"
#include "GameModes/CTF/Actors/CTF_Base.h"

#include "TimerManager.h"

ACTF_GameMode::ACTF_GameMode()
{
	//bStartPlayersAsSpectators = true;
	//bDelayedStart = true;
}

void ACTF_GameMode::PostLogin(APlayerController* NewPlayer)
{
	
	Super::PostLogin(NewPlayer);
}

void ACTF_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACTF_GameMode, FlagActor);

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

	if (CtfPlayerState->GetTeam() == ETeam::None)
	{


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
	FindInitialFlag();
}

void ACTF_GameMode::FindInitialFlag()
{
	FlagActor = Cast< ACTF_Flag> (UGameplayStatics::GetActorOfClass(GetWorld(), ACTF_Flag::StaticClass()));
	InitalFlagTransform = FlagActor->GetActorTransform();
}

void ACTF_GameMode::OnFlagPickedUp(APawn* PlayerPawn, ACTF_Flag* Flag)
{
	if (FlagActor && FlagActor == Flag)
	{
		// Hide the flag and attach it to the player
		//FlagActor->SetActorHiddenInGame(true);
		if (ACharacter* PCharacter = Cast<ACharacter>(PlayerPawn)) 
		{
			FlagActor->AttachToComponent(PCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FlagSocket"));
			FlagActor->SetOwner(PlayerPawn);
			

			FlagPlayer = Cast<APlayerController>(PlayerPawn->GetController());
			if (FlagPlayer)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("FLAG PICKED")));
			}
		}
		
		
	}
}

void ACTF_GameMode::OnFlagDropped()
{
	//TODO Do a trace to Adjust Flag position
	FlagActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FlagActor->OnFlagDropped();
	FlagPlayer = nullptr;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Flag Dropped")));
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
			OnFlagDropped();
			ResetFlag();
			CheckWinCondition();		
		}
	}
}

void ACTF_GameMode::ResetFlag_Implementation()
{
	// Reset the flag to the center of the map
	if (FlagActor)
	{
		
		FlagActor->SetActorHiddenInGame(false);
		FlagActor->SetOwner(nullptr);
		FlagActor->SetActorTransform(InitalFlagTransform);
		FlagActor->EnableFlagCollision();
		
	}
}

void ACTF_GameMode::ResetThisGame()
{
	// Reset scores to 0
	// ResetFlag();
	RestartGame();
}
/*
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
*/
void ACTF_GameMode::RespawnPlayer(AController* CtfController)
{
	
	//
	
	if (CtfController)
	{
		// Restart the player, which will spawn a new character for them.
		//RestartPlayer(CtfController);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("DEAD RESPAWNING")));
		APawn* OldPawn = CtfController->GetPawn();

		RespawningPlayers.Remove(CtfController);
		CtfController->UnPossess();
		OldPawn->Destroy();

		
		RestartPlayer(CtfController);
		/*
		
		*/
	}
	
	
}

void ACTF_GameMode::PlayerDied(AController* CtfController)
{
	
	
	if (CtfController )
	{		
		RespawningPlayers.AddUnique(CtfController);
		
		if (FlagPlayer)
		{
			APawn* LocalPawn = CtfController->GetPawn();
			APlayerController* PC = Cast<APlayerController>(LocalPawn->GetController());

			if (LocalPawn && PC && PC == FlagPlayer)
			{
				OnFlagDropped();
				FlagActor->EnableFlagCollision();
			}
			
		}
			
		
		FTimerHandle RespawnTimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("RespawnPlayer"), CtfController);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, TimerDel, 3.0f, false);
	}
	
}

APlayerController* ACTF_GameMode::GetFlagPlayer()
{
	return FlagPlayer;
}

bool ACTF_GameMode::IsPlayerRespawning(AController* PC)
{
	if (!PC) return false;
	
	return RespawningPlayers.Contains(PC);
}

void ACTF_GameMode::OnRep_FlagActor()
{
	//Flag actor needs to be replicated in order to replicate the location changes
	// Can be used for VFX on clients when the Flag Actor Changes
}

void ACTF_GameMode::CheckWinCondition()
{
	ACTF_GameState* PGameState = GetGameState<ACTF_GameState>();
	ETeam WinnerTeam;
	bool LMatchHasEnded = false;
	if (PGameState)
	{	
		if (PGameState->RedTeamScore >= 3)
		{	
			//OnMatchEnded.Broadcast(ETeam::RedTeam);
			WinnerTeam = ETeam::RedTeam;
			LMatchHasEnded = true;
		}
		else if (PGameState->BlueTeamScore >= 3)
		{
			//OnMatchEnded.Broadcast(ETeam::BlueTeam);
			WinnerTeam = ETeam::BlueTeam;
			LMatchHasEnded = true;
		}
	}
	if (LMatchHasEnded)
	{
		//
		PGameState->MatchEnded(WinnerTeam);

		//DisablePlayerInput();
		//RestartGame();
		
		

		FTimerHandle DisableInputTimerHandle;
		GetWorldTimerManager().SetTimer(DisableInputTimerHandle, this, &ACTF_GameMode::DisablePlayerInput, 1.f, false, 1.0f);

		FTimerHandle RespawnTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&ACTF_GameMode::ResetThisGame,3.f,false,1.0f);
		
		//DisablePlayerInput();
	}

	
	//ResetGame();
}

void ACTF_GameMode::DisablePlayerInput_Implementation()
{
	ACTF_GameState* PGameState = GetGameState<ACTF_GameState>();
	for (APlayerState* LoopPS : PGameState->PlayerArray)
	{
		if (ACTF_PlayerController* PlayerController = Cast<ACTF_PlayerController>(LoopPS->GetOwner()))
		{
			PlayerController->DisablePlayerInput();
			//PlayerController->UnPossess();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("DISABLE INPUT")));
			//RestartPlayer(PlayerController);
		}
	}
}

void ACTF_GameMode::CTF_OnPlayerDeath_Implementation(AController* InController)
{
	PlayerDied(InController);
}