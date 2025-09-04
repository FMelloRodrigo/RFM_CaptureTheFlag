// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_PlayerState.h"
#include "GameModes/CTF/Interfaces/ICTF_Teams.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"


ACTF_PlayerState::ACTF_PlayerState()
{
    Team = ETeam::None;
}

void ACTF_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    
    DOREPLIFETIME(ACTF_PlayerState, Team);
}

void ACTF_PlayerState::SetTeam(ETeam NewTeam)
{
    

    if (HasAuthority())
    {
        Team = NewTeam;
        ForceNetUpdate();
        //For server players
        //SendTeamChangeMessage();
    }
}

ETeam ACTF_PlayerState::GetTeam() const
{
    return Team;
}

void ACTF_PlayerState::OnRep_Team()
{
   
    // This function is called on clients when the Team variable is replicated.
    // You can add logic here if you need to do something on the client
    // when a player's team is set, e.g., changing their character's material color.
    // APlayerState's owner is an APlayerController.
    SendTeamChangeMessage();
    
    
}

void ACTF_PlayerState::SendTeamChangeMessage()
{
    if (GetPawn())
    {
        if (GetPawn()->Implements<UICTF_Teams>())
        {
            IICTF_Teams::Execute_OnTeamsChanged(GetPawn(), GetTeam());
            GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("Current State: %s"), *UEnum::GetValueAsString(Team)));

        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("REPLICATION OF PAWN ON PLAYER STATE NOT VALID")));
    }

}

