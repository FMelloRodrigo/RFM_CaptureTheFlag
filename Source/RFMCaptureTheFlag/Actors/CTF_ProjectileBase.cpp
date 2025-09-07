// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CTF_ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "Sound/SoundCue.h"

// Sets default values
ACTF_ProjectileBase::ACTF_ProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//SetReplicateMovement(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(5.0f);
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentHit.AddDynamic(this, &ACTF_ProjectileBase::OnHit);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = ProjSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = ProjShouldBounce;
	ProjectileMovementComponent->ProjectileGravityScale = ProjGravityScale;

	

	InitialLifeSpan = 10.0f;

}

void ACTF_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACTF_ProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor != GetOwner()))
	{
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
		if (ASI)
		{
			UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent();
			if (TargetASC && DamageEffect)
			{
				FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
				ContextHandle.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1, ContextHandle);
				if (SpecHandle.IsValid())
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					Multicast_PlayHitEffects(Hit.Location);
				}
			}
		}	
		Destroy();
	}
}

void ACTF_ProjectileBase::Multicast_PlayHitEffects_Implementation(const FVector& Location)
{
	PlayHitEffects(Location);
}

void ACTF_ProjectileBase::PlayHitEffects(const FVector& Location)
{
	if (HitParticleSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticleSystem, Location, GetActorRotation());
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, Location, 0.5);
	}

}

void ACTF_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


