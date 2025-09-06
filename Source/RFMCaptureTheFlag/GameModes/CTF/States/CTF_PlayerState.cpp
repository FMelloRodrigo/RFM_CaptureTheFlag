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
    }
}

ETeam ACTF_PlayerState::GetTeam() const
{
    return Team;
}

void ACTF_PlayerState::OnRep_Team()
{
   
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

    }
}