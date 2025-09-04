// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CTF_GameMode.generated.h"

class ACTF_Flag;

UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_GameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ACTF_GameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	// virtual bool ReadyToStartMatch_Implementation() override;



	// Called when a player picks up the flag
	UFUNCTION(BlueprintCallable, Category = "CTF")
	void OnFlagPickedUp(APawn* PlayerPawn, ACTF_Flag* Flag);

	// Called when a player scores
	UFUNCTION(BlueprintCallable, Category = "CTF")
	void OnScore(APlayerController* Scorer);

protected:
	// The flag actor in the world
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "CTF")
	ACTF_Flag* FlagActor;

	// Spawns the flag at the center of the map
	void FindInitialFlag();

	void PlayerKilled(AController* Killer, AController* Victim);



	// Resets the flag to its original location
	void ResetFlag();

	// Resets the game state
	void ResetGame();

	virtual void HandleMatchHasStarted() override;

	
	
};
