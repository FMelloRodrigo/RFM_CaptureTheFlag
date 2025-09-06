// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameModes/CTF/Interfaces/ICTF_Teams.h"
#include "CTF_PlayerController.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInterface;

UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_PlayerController : public APlayerController
{
	GENERATED_BODY()

private:


public:
	// The team color logic is now on the player controller
	// This function will be called by the PlayerState to update the character's material.
	UFUNCTION(BlueprintCallable, Category = "Teams")
	void UpdateCharacterTeamColor(ETeam NewTeam);

	virtual void OnUnPossess() override;

	void CreatePlayerHUD();
	void CreateMatchHUD();

	UFUNCTION(Client, Reliable)
	void DisablePlayerInput();

protected:
	
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// The dynamic material instance created at runtime to apply the team color.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
	UMaterialInstanceDynamic* TeamMaterialInstance;

	// Base material with a color parameter (e.g., "TeamColor") that can be changed.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInterface* BaseTeamMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	FName ColorMaterialName;


};
