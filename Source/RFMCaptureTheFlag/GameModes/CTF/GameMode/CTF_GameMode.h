// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/Character.h"
#include "GameModes/CTF/States/CTF_PlayerState.h"
#include "GameModes/CTF/Interfaces/ICTF_GameMode.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "CTF_GameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchEnded, ETeam, WinnerTeam);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStarted);

class ACTF_Flag;


UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_GameMode : public AGameMode, public IICTF_GameMode
{
	GENERATED_BODY()

public:

	ACTF_GameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Logout(AController* Exiting) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(AController* CtfController);

	UFUNCTION()
	void OnFlagPickedUp(APawn* PlayerPawn, ACTF_Flag* Flag);
	UFUNCTION()
	void OnFlagDropped();

	UFUNCTION(BlueprintCallable, Category = "CTF")
	void OnScore(APlayerController* Scorer);

	APlayerController* GetFlagPlayer();

	bool IsPlayerRespawning(AController* PC);

	UPROPERTY(BlueprintAssignable, Category = "CTF")
	FOnMatchEnded OnMatchEnded;

	UPROPERTY(BlueprintAssignable, Category = "CTF")
	FOnMatchStarted OnMatchStarted;

	virtual void CTF_OnPlayerDeath_Implementation(AController* InController) override;


protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "CTF", ReplicatedUsing = OnRep_FlagActor)
	ACTF_Flag* FlagActor;

	UFUNCTION()
	void OnRep_FlagActor();

	void FindInitialFlag();

	UPROPERTY()
	FTransform InitalFlagTransform;

	UFUNCTION()
	void RespawnPlayer(AController* CtfController);


	UFUNCTION(NetMulticast, Reliable)
	void ResetFlag();


	void ResetThisGame();

	virtual void HandleMatchHasStarted() override;

	APlayerController* FlagPlayer;
	
	UFUNCTION(NetMulticast, Reliable)
	void DisablePlayerInput();

private:

	TArray<AController*> RespawningPlayers;

	void CheckWinCondition();

	void AlignFlagWithFloor();

};
