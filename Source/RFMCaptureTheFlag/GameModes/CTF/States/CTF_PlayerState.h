// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameModes/CTF/CTFTeams.h"
#include "CTF_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    ACTF_PlayerState();

    

    // Setter for Team - Called on the server
    void SetTeam(ETeam NewTeam);

    // Getter for Team
    UFUNCTION(BlueprintCallable, Category = "Teams")
    ETeam GetTeam() const;

private:
    // The player's current team
    UPROPERTY(ReplicatedUsing = OnRep_Team)
    ETeam Team = ETeam::None;

    // Replication notification function for Team
    UFUNCTION()
    void OnRep_Team();

    void SendTeamChangeMessage();
    //void RestartPlayerOnServer();

    /*
    UFUNCTION(Server, Reliable)
    void OnTeamChanged_Server();
    */

protected:
    // Replication logic
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
	

