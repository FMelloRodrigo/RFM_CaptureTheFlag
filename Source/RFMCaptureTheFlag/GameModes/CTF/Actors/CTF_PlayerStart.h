// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "GameModes/CTF/CTFTeams.h"
#include "CTF_PlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_PlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, Category = "Team")
	ETeam Team;
	
};
