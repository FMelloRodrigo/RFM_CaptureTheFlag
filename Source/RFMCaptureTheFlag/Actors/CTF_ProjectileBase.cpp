// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CTF_ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"

// Sets default values
ACTF_ProjectileBase::ACTF_ProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(5.0f);
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->OnComponentHit.AddDynamic(this, &ACTF_ProjectileBase::OnHit);
	RootComponent = SphereComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = SphereComponent;
	ProjectileMovementComponent->InitialSpeed = ProjSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

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
				}
			}
		}

		Destroy();
	}
}

void ACTF_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


