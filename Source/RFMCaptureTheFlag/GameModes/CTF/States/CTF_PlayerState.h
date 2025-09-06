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

    void SetTeam(ETeam NewTeam);

    
   UFUNCTION(BlueprintCallable, Category = "Teams")
    ETeam GetTeam() const;

private:

    UPROPERTY(ReplicatedUsing = OnRep_Team)
    ETeam Team = ETeam::None;


    UFUNCTION()
    void OnRep_Team();

    UFUNCTION()
    void PSPawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);
    
    void SendTeamChangeMessage();

protected:

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
};
	

