// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_GameState.h"

#include "Controllers/CTF_PlayerController.h"



ACTF_GameState::ACTF_GameState()
{
	// Initialize scores
	RedTeamScore = 0;
	BlueTeamScore = 0;
	bReplicates = true;
	WinnerTeam = ETeam::None;
	bMatchedHasEnded = false;
}


void ACTF_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate team players arrays and scores
	DOREPLIFETIME(ACTF_GameState, RedTeamPlayers);
	DOREPLIFETIME(ACTF_GameState, BlueTeamPlayers);

	DOREPLIFETIME_CONDITION_NOTIFY(ACTF_GameState, RedTeamScore, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ACTF_GameState, BlueTeamScore, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ACTF_GameState, WinnerTeam, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ACTF_GameState, bMatchedHasEnded, COND_None, REPNOTIFY_Always);
}

void ACTF_GameState::OnRep_RedTeamScore()
{
	// This function is called on all clients when RedTeamScore is replicated.
	OnTeamScored.Broadcast(RedTeamScore, BlueTeamScore, ETeam::RedTeam);
}

void ACTF_GameState::OnRep_BlueTeamScore()
{
	// This function is called on all clients when BlueTeamScore is replicated.
	OnTeamScored.Broadcast(RedTeamScore, BlueTeamScore, ETeam::BlueTeam);
}



void ACTF_GameState::AddPlayerToTeam(ACTF_PlayerState* PlayerState, ETeam Team)
{
	
	 
	if (PlayerState)

	{
		if (Team == ETeam::RedTeam)
		{
			RedTeamPlayers.AddUnique(PlayerState);
			
		}
		else if (Team == ETeam::BlueTeam)
		{
			BlueTeamPlayers.AddUnique(PlayerState);
			
		}
	}
	
}

void ACTF_GameState::AddScoreToTeam(ETeam Team, int32 ScoreToAdd)
{
	if (Team == ETeam::RedTeam)
	{
		RedTeamScore += ScoreToAdd;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("SCORED: %s"), *UEnum::GetValueAsString(Team)));
	}
	else if (Team == ETeam::BlueTeam)
	{
		BlueTeamScore += ScoreToAdd;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("SCORED: %s"), *UEnum::GetValueAsString(Team)));
	}
	// For server players
	OnTeamScored.Broadcast(RedTeamScore, BlueTeamScore, Team);
}



void ACTF_GameState::MatchEnded(ETeam Team)
{
	WinnerTeam = Team;
	//For Server
	OnMatchEnded.Broadcast(WinnerTeam);
	bMatchedHasEnded = true;
}
void ACTF_GameState::OnRep_WinnerTeam()
{
	OnMatchEnded.Broadcast(WinnerTeam);
}

void ACTF_GameState::OnRep_MatchedHasEnded()
{
	OnMatchEnded.Broadcast(WinnerTeam);
}
