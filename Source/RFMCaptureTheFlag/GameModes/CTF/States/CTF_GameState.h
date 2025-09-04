// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "CTF_GameState.generated.h"


UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_GameState : public AGameState
{
	GENERATED_BODY()

public:
	ACTF_GameState();

	// Function to add a player to a team
	void AddPlayerToTeam(ACTF_PlayerState* PlayerState, ETeam Team);

	// Function to increment score for a team
	UFUNCTION(BlueprintCallable, Category = "Teams")
	void AddScoreToTeam(ETeam Team, int32 ScoreToAdd);

	// Replicated array for players on the red team
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Teams")
	TArray<class ACTF_PlayerState*> RedTeamPlayers;

	// Replicated array for players on the blue team
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Teams")
	TArray<class ACTF_PlayerState*> BlueTeamPlayers;

	// Replicated score for the red team
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Teams")
	int32 RedTeamScore;

	// Replicated score for the blue team
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Teams")
	int32 BlueTeamScore;

protected:
	// Override to handle variable replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
