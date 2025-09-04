// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameModes/CTF/CTFTeams.h"
#include "ICTF_Teams.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICTF_Teams : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RFMCAPTURETHEFLAG_API IICTF_Teams
{
	GENERATED_BODY()



public:

	//virtual void OnTeamsChangedNative() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CTF_Teams")
	void OnTeamsChanged(ETeam PlayerTeam);
	
	virtual void OnTeamsChanged_Implementation(ETeam PlayerTeam) {}


};
