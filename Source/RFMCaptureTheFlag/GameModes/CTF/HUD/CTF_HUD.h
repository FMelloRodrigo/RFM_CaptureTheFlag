// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTF_HUD.generated.h"

/**
 * 
 */
UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_HUD : public AHUD
{
	GENERATED_BODY()

	ACTF_HUD();

public:
	void CreatePlayerHUD();
	void RemovePlayerHUD();
	void CreateMatchHUD();

private:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> MatchHUDWidgetClass;

	UPROPERTY()
	UUserWidget* PlayerHUDWidget;

	UPROPERTY()
	UUserWidget* MatchHUDWidget;
	
};
