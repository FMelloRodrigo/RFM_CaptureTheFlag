// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CTF/HUD/CTF_HUD.h"

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

void ACTF_HUD::RemovePlayerHUD()
{
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->RemoveFromParent();
		PlayerHUDWidget = nullptr;
	}
}

void ACTF_HUD::CreateMatchHUD()
{
	if (MatchHUDWidget)
	{
		return;
	}

	if (MatchHUDWidgetClass)
	{
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			MatchHUDWidget = CreateWidget<UUserWidget>(PC, MatchHUDWidgetClass);
			if (MatchHUDWidget)
			{
				MatchHUDWidget->AddToViewport();
			}
		}
	}

}
