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
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("CREATE HUD")));
	if (PlayerHUDWidget)
	{
		return;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("HUD ALREADY VALID")));
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
