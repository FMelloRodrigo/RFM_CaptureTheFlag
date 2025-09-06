// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "CTF_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTeamScored, int32, RedTeamScore, int32, BlueTeamScore, ETeam, TeamThatScored);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchHasEnded, ETeam, WinnerTeam);

UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_GameState : public AGameState
{
	GENERATED_BODY()

public:
	ACTF_GameState();

	void AddPlayerToTeam(ACTF_PlayerState* PlayerState, ETeam Team);

	void MatchEnded(ETeam Team);

	UFUNCTION(BlueprintCallable, Category = "Teams")
	void AddScoreToTeam(ETeam Team, int32 ScoreToAdd);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Teams")
	TArray<class ACTF_PlayerState*> RedTeamPlayers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Teams")
	TArray<class ACTF_PlayerState*> BlueTeamPlayers;

	UPROPERTY(BlueprintAssignable, Category = "Teams")
	FOnTeamScored OnTeamScored;

	UPROPERTY(BlueprintAssignable, Category = "Teams")
	FOnMatchHasEnded OnMatchEnded;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Teams", ReplicatedUsing = OnRep_RedTeamScore)
	int32 RedTeamScore;

	// Replicated score for the blue team
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Teams", ReplicatedUsing = OnRep_BlueTeamScore)
	int32 BlueTeamScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Teams", ReplicatedUsing = OnRep_WinnerTeam)
	ETeam WinnerTeam;


protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_RedTeamScore();
	UFUNCTION()
	void OnRep_BlueTeamScore();
	UFUNCTION()
	void OnRep_WinnerTeam();
	
};
