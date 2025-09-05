// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_GameState.h"
#include "Net/UnrealNetwork.h"



ACTF_GameState::ACTF_GameState()
{
	// Initialize scores
	RedTeamScore = 0;
	BlueTeamScore = 0;
	bReplicates = true;
}

void ACTF_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate team players arrays and scores
	DOREPLIFETIME(ACTF_GameState, RedTeamPlayers);
	DOREPLIFETIME(ACTF_GameState, BlueTeamPlayers);
	DOREPLIFETIME(ACTF_GameState, RedTeamScore);
	DOREPLIFETIME(ACTF_GameState, BlueTeamScore);
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
	}
	else if (Team == ETeam::BlueTeam)
	{
		BlueTeamScore += ScoreToAdd;
	}
}
