// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModes/CTF/CTFTeams.h"
#include "CTF_Base.generated.h"

class UBoxComponent;

UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_Base : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACTF_Base();

protected:
	// The collision component for the base
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;

	// The team this base belongs to. Set in the editor.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	ETeam Team;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap function to check for scoring
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
