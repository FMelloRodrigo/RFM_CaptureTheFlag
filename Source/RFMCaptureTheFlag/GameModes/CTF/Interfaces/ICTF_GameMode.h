// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICTF_GameMode.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICTF_GameMode : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RFMCAPTURETHEFLAG_API IICTF_GameMode
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CTF_Teams")
	void CTF_OnPlayerDeath(AController* InController);

	virtual void CTF_OnPlayerDeath_Implementation(AController* InController) {}
};
