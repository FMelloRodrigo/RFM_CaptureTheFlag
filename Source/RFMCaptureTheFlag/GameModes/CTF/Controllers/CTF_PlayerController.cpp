// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CTF/Controllers/CTF_PlayerController.h"
#include "GameModes/CTF/HUD/CTF_HUD.h"
#include "Characters/CTFCharacter.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "GameModes/CTF/GameMode/CTF_GameMode.h"


void ACTF_PlayerController::BeginPlay()
{
	Super::BeginPlay();
  
}
void ACTF_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); 
	
}
	

void ACTF_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}
void ACTF_PlayerController::OnUnPossess()
{  
    if (ACTF_HUD* CTFHUD = Cast<ACTF_HUD>(GetHUD()))
    {
        CTFHUD->RemovePlayerHUD();
    }
    Super::OnUnPossess();
}

void ACTF_PlayerController::CreatePlayerHUD()
{
        if (ACTF_HUD* CTFHUD = Cast<ACTF_HUD>(GetHUD()))
        {
            // Tell the HUD to create and display its widgets.
            CTFHUD->CreatePlayerHUD();
        } 
}

void ACTF_PlayerController::CreateMatchHUD()
{
    if (ACTF_HUD* CTFHUD = Cast<ACTF_HUD>(GetHUD()))
    {
        // Tell the HUD to create and display its widgets.
        CTFHUD->CreateMatchHUD();
        
    }

}

void ACTF_PlayerController::DisablePlayerInput_Implementation()
{
    if (GetPawn())
    {
        GetPawn()->DisableInput(this); 
    }
}

void ACTF_PlayerController::CTF_InitPlayerInput_Implementation()
{
    CreatePlayerHUD();
}
