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
	
	ACTF_Flag();

protected:
	
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FlagMesh;

	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	
	UFUNCTION()
	void OnFlagDropped();
	UFUNCTION()
	void EnableFlagCollision();

private:
	
	UPROPERTY(Replicated)
	bool bIsHeld;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void FlagPickedEvents();

};
