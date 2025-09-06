// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_PlayerState.h"
#include "GameModes/CTF/Interfaces/ICTF_Teams.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "GameModes/CTF/Controllers/CTF_PlayerController.h"
#include "Characters/CTFCharacter.h"
#include "TimerManager.h"


ACTF_PlayerState::ACTF_PlayerState()
{
    Team = ETeam::None;
    bReplicates = true;
    OnPawnSet.AddDynamic(this, &ACTF_PlayerState::PSPawnSet);
}

void ACTF_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    
    DOREPLIFETIME(ACTF_PlayerState, Team);
}

void ACTF_PlayerState::BeginPlay()
{
    
}

void ACTF_PlayerState::SetTeam(ETeam NewTeam)
{
    

    if (HasAuthority())
    {
        Team = NewTeam;
       // ForceNetUpdate();
        //For server players
        //SendTeamChangeMessage();
        //OnRep_Team();
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
    //SendTeamChangeMessage();
    
}

void ACTF_PlayerState::PSPawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{

    
      SendTeamChangeMessage();

    
}




void ACTF_PlayerState::SendTeamChangeMessage()
{
    
    if (GetPawn())
    {
        ACTFCharacter* PossessedCharacter = Cast<ACTFCharacter>(GetPawn());
        PossessedCharacter->UpdateCharacterTeamColor(Team);

        if (ACTF_PlayerController* PC = Cast<ACTF_PlayerController>(GetOwner()))
        {
            PC->CreateMatchHUD();
            
        }
        //GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("UPDATE TEAM COLOR")));
    }
    else

    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("PAWN NOT VALID ON PLAYER STATE")));
    }
    
    
    
    
    
    
    /*
    
    if (ACTF_PlayerController* CTFPC = Cast<ACTF_PlayerController>(GetOwner()))
    {
        CTFPC->UpdateCharacterTeamColor(Team);
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Team Color Updated via OnRep_Team: %s"), *UEnum::GetValueAsString(Team)));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("REPLICATION OF PAWN ON PLAYER STATE NOT VALID")));
    }
     */
}
    /*
    if (ACTF_PlayerController* CTFPC = Cast<ACTF_PlayerController>(GetOwner()))
    {
        CTFPC->UpdateCharacterTeamColor(Team);
        GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Green, FString::Printf(TEXT("Team Color Updated via OnRep_Team: %s"), *UEnum::GetValueAsString(Team)));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Red, FString::Printf(TEXT("SendTeamChangeMessage: Invalid PlayerController")));
    }
    */
    /*
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("TEAM MESSAGE")));
    if (GetOwner())
    {
        if (ACTF_PlayerController* PC = Cast<ACTF_PlayerController>(GetOwner()))
        {
            PC->UpdateCharacterTeamColor(Team);

        }
    }
 */


//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("PLAYER NOT VALID ON PLAYER STATE")));
//GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("REPLICATION OF PAWN ON PLAYER STATE NOT VALID")));
//GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("Current State: %s"), *UEnum::GetValueAsString(Team)));