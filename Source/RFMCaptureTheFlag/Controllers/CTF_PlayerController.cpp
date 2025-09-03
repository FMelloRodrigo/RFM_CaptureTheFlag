// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/CTF_PlayerController.h"
#include "HUD/CTF_HUD.h"

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
