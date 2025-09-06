// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CTF/GameMode/CTF_GameMode.h"

#include "GameModes/CTF/States/CTF_GameState.h"
#include "GameModes/CTF/Actors/CTF_PlayerStart.h"
#include "GameModes/CTF/Controllers/CTF_PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "GameModes/CTF/Actors/CTF_Flag.h"
#include "GameModes/CTF/Actors/CTF_Base.h"

#include "TimerManager.h"


ACTF_GameMode::ACTF_GameMode()
{
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

// Finds the best team ( the one with less members) and assign the player
// Finds a random Player Start of that team 
AActor* ACTF_GameMode::ChoosePlayerStart_Implementation(AController* Player)
{	
	ACTF_GameState* CtfGameState = GetGameState<ACTF_GameState>();
	ACTF_PlayerState* CtfPlayerState = Cast<ACTF_PlayerState>(Player->PlayerState);
	
	if (!CtfPlayerState || !CtfGameState)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTF_PlayerStart::StaticClass(), PlayerStarts);

	if (CtfPlayerState->GetTeam() == ETeam::None)
	{

		if (CtfGameState->RedTeamPlayers.Num() <= CtfGameState->BlueTeamPlayers.Num())
		{
			
			CtfPlayerState->SetTeam(ETeam::RedTeam);
			CtfGameState->AddPlayerToTeam(CtfPlayerState, ETeam::RedTeam);
			for (AActor* StartActor : PlayerStarts)
			{
				ACTF_PlayerStart* PlayerStart = Cast<ACTF_PlayerStart>(StartActor);

				if (PlayerStart && PlayerStart->Team == ETeam::RedTeam)
				{				
					return PlayerStart;
				}
			}
		}
		else
		{			
			CtfPlayerState->SetTeam(ETeam::BlueTeam);
			CtfGameState->AddPlayerToTeam(CtfPlayerState, ETeam::BlueTeam);

			for (AActor* StartActor : PlayerStarts)
			{
				ACTF_PlayerStart* PlayerStart = Cast<ACTF_PlayerStart>(StartActor);

				if (PlayerStart && PlayerStart->Team == ETeam::BlueTeam)
				{
					
					return PlayerStart;
				}
			}
		}
	}
	
	// Did not found start point, returns the default
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ACTF_GameMode::HandleMatchHasStarted()
{
	
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

		if (ACharacter* PCharacter = Cast<ACharacter>(PlayerPawn)) 
		{
			FlagActor->AttachToComponent(PCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FlagSocket"));
			FlagActor->SetOwner(PlayerPawn);
			FlagPlayer = Cast<APlayerController>(PlayerPawn->GetController());

		}
		
		
	}
}

void ACTF_GameMode::OnFlagDropped()
{
	FlagActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FlagActor->OnFlagDropped();
	AlignFlagWithFloor();
	FlagPlayer = nullptr;
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
	RestartGame();
}

void ACTF_GameMode::RespawnPlayer(AController* CtfController)
{
	if (CtfController)
	{
		APawn* OldPawn = CtfController->GetPawn();
		RespawningPlayers.Remove(CtfController);
		CtfController->UnPossess();
		OldPawn->Destroy();		
		RestartPlayer(CtfController);

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
			WinnerTeam = ETeam::RedTeam;
			LMatchHasEnded = true;
		}
		else if (PGameState->BlueTeamScore >= 3)
		{
			WinnerTeam = ETeam::BlueTeam;
			LMatchHasEnded = true;
		}
	}
	if (LMatchHasEnded)
	{		
		PGameState->MatchEnded(WinnerTeam);

		FTimerHandle DisableInputTimerHandle;
		GetWorldTimerManager().SetTimer(DisableInputTimerHandle, this, &ACTF_GameMode::DisablePlayerInput, 1.f, false, 1.0f);

		FTimerHandle RespawnTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle,this,&ACTF_GameMode::ResetThisGame,3.f,false,1.0f);
			
	}	
}

void ACTF_GameMode::DisablePlayerInput_Implementation()
{
	ACTF_GameState* PGameState = GetGameState<ACTF_GameState>();
	for (APlayerState* LoopPS : PGameState->PlayerArray)
	{
		if (ACTF_PlayerController* PlayerController = Cast<ACTF_PlayerController>(LoopPS->GetOwner()))
		{
			PlayerController->DisablePlayerInput();
			
		}
	}
}

void ACTF_GameMode::CTF_OnPlayerDeath_Implementation(AController* InController)
{
	PlayerDied(InController);
}

void ACTF_GameMode::AlignFlagWithFloor()
{
	const FVector Start = FlagActor->GetActorLocation();
	const FVector End = Start + FVector (0,0,-500);
	FHitResult Hit;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.f);

	bool bHit = GetWorld()->SweepSingleByChannel(Hit,Start,End,FQuat::Identity,ECC_WorldStatic,Sphere);
	if (bHit)
	{
		const FVector SetLoc = Hit.ImpactPoint;			 
		const FRotator SetRot = FRotator( 0, 0, 0 );
		FlagActor->SetActorLocationAndRotation(SetLoc, SetRot);
	}


}