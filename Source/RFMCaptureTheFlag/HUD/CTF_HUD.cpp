// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CTF_HUD.h"

#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

ACTF_HUD::ACTF_HUD()
{
	
}

void ACTF_HUD::BeginPlay()
{
	Super::BeginPlay();

}

void ACTF_HUD::CreatePlayerHUD()
{
	// Don't create the widget if it already exists
	if (PlayerHUDWidget)
	{
		return;
	}

	if (PlayerHUDWidgetClass)
	{
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PlayerHUDWidget = CreateWidget<UUserWidget>(PC, PlayerHUDWidgetClass);
			if (PlayerHUDWidget)
			{
				PlayerHUDWidget->AddToViewport();
			}
		}
	}
}