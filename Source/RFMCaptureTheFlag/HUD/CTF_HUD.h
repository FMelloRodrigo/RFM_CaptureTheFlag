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


private:
	virtual void BeginPlay() override;

	// The class of the main player HUD widget to create
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> PlayerHUDWidgetClass;

	// The instance of the created widget
	UPROPERTY()
	UUserWidget* PlayerHUDWidget;
	
};
