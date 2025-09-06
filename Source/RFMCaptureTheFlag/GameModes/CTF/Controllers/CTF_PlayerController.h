// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameModes/CTF/Interfaces/ICTF_Teams.h"
#include "GameModes/CTF/Interfaces/ICTF_PlayerController.h"
#include "CTF_PlayerController.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInterface;

UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_PlayerController : public APlayerController, public IICTF_PlayerController
{
	GENERATED_BODY()

private:


public:

	virtual void OnUnPossess() override;

	void CreatePlayerHUD();
	void CreateMatchHUD();

	UFUNCTION(Client, Reliable)
	void DisablePlayerInput();

protected:
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void CTF_InitPlayerInput_Implementation() override;


};
