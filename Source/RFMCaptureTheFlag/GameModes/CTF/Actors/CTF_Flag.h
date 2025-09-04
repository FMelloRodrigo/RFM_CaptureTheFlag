// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTF_Flag.generated.h"

class USphereComponent;
class ACTF_PlayerState;

UCLASS()
class RFMCAPTURETHEFLAG_API ACTF_Flag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTF_Flag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The collision component for the flag
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	// The static mesh component for the flag
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FlagMesh;

	// Overlap function to detect when a player walks over the flag
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Public function to drop the flag at the current location
	UFUNCTION(BlueprintCallable, Category = "CTF")
	void OnFlagDropped();

private:
	// A boolean to track if the flag is currently held by a player
	UPROPERTY(Replicated)
	bool bIsHeld;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
