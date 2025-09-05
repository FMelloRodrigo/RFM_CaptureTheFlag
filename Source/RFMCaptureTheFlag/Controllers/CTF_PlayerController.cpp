// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/CTF_PlayerController.h"
#include "HUD/CTF_HUD.h"
#include "Characters/CTFCharacter.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"

void ACTF_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// We only want to create the HUD on the client that owns this controller.
	// IsLocalController() is the perfect check for this.
	if (IsLocalController())
	{
		// APlayerController has a built-in reference to its HUD.
		// We get it and cast it to our specific FPSHUD class.
		if (ACTF_HUD* CTFHUD = Cast<ACTF_HUD>(GetHUD()))
		{
			// Tell the HUD to create and display its widgets.
			CTFHUD->CreatePlayerHUD();
		}
        
	}
}

void ACTF_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	/*
	ACTF_PlayerState* CtfPlayerState = Cast<ACTF_PlayerState>(InPawn->GetPlayerState());
	GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Green, FString::Printf(TEXT("ON POSSES")));
	if (CtfPlayerState && InPawn)
	{
		UpdateCharacterTeamColor(CtfPlayerState->GetTeam());
		
		//GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Yellow, FString::Printf(TEXT("Current State: %s"), *UEnum::GetValueAsString(CtfPlayerState->GetTeam())));
		GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Green, FString::Printf(TEXT("VALID ON POSSES")));

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 55.f, FColor::Red, FString::Printf(TEXT("NOT VALID REPLICATE ON POSSES")));
	}
	*/
	
}
	

void ACTF_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	//DOREPLIFETIME(ACTF_PlayerController, TeamSet);

}

void ACTF_PlayerController::UpdateCharacterTeamColor(ETeam NewTeam)
{
	
    if (ACTFCharacter* PossessedCharacter = Cast<ACTFCharacter>(GetPawn()))
    {
        USkeletalMeshComponent* Mesh = PossessedCharacter->GetMesh();
        if (Mesh && BaseTeamMaterial)
        {
            if (!TeamMaterialInstance)
            {
                TeamMaterialInstance = UMaterialInstanceDynamic::Create(BaseTeamMaterial, this);
                Mesh->SetMaterial(0, TeamMaterialInstance);
                PossessedCharacter->FirstPersonMesh->SetMaterial(0, TeamMaterialInstance);
            }

            FLinearColor TeamColor;
            switch (NewTeam)
            {
            case ETeam::RedTeam:
                TeamColor = FLinearColor::Red;
                break;
            case ETeam::BlueTeam:
                TeamColor = FLinearColor::Blue;
                break;
            default:
                TeamColor = FLinearColor::White;
                break;
            }

            if (TeamMaterialInstance)
            {
                TeamMaterialInstance->SetVectorParameterValue(FName("Paint Tint"), TeamColor);
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Team Color Updated: %s"), *UEnum::GetValueAsString(NewTeam)));
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("UpdateCharacterTeamColor: Invalid Mesh or BaseTeamMaterial")));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("UpdateCharacterTeamColor: Invalid Pawn")));
        
    }
	
	
}

/*

FTimerHandle MyTimerHandle;
        GetWorldTimerManager().SetTimer(
            MyTimerHandle,                      // Handle to manage the timer
            this,                               // Object that owns the function
            &ACTF_PlayerController::RetrySetMaterial,         // Function to call
            0.25f,                               // Time in seconds
            false,                               // Loop? true = repeat, false = once
            0.0f                                // Initial delay before first call
        );

*/