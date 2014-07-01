// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ActionRPGGame.h"

#include "../Types/AREnumTypes.h"
#include "Net/UnrealNetwork.h"

#include "../BlueprintLibrary/ARTraceStatics.h"
#include "../ARCharacter.h"
#include "ParticleDefinitions.h"

#include "ARFXEffectComponent.h"

UARFXEffectComponent::UARFXEffectComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UARFXEffectComponent::SpawnTrailEffect_Implementation(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName, APawn* Causer)
{
	FVector Origin = UARTraceStatics::GetStartLocation(SocketName, Causer);
	if (target.GetActor())
	{
		if (trailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(Causer, trailFX, Origin);
			if (TrailPSC)
			{
				const FVector AdjustedDir = (target.ImpactPoint - Origin).SafeNormal();
				FVector ParticleSpeed = AdjustedDir * trailSpeed;
				//TrailPSC->SetVectorParameter(trailSpeedParam, ParticleSpeed);
			}
		}
	}
}

void UARFXEffectComponent::AttachEffectToTarget_Implementation(UParticleSystem* FXIn, FHitResult Target, FName AttachSocket, APawn* Causer)
{
	if (!Target.GetActor() && !FXIn)
		return;

	AARCharacter* hitTarget = Cast<AARCharacter>(Target.GetActor());
	if (!hitTarget)
		return;

	UParticleSystemComponent* AttachedPSC = UGameplayStatics::SpawnEmitterAttached(FXIn, hitTarget->Mesh, AttachSocket);
}

void UARFXEffectComponent::SpawnEffectOnHitLoc_Implementation(UParticleSystem* FXIn, FHitResult HitLocation, APawn* Causer)
{
	if (!HitLocation.GetActor() && !FXIn)
		return;

	UParticleSystemComponent* ImpactPSC = UGameplayStatics::SpawnEmitterAtLocation(HitLocation.GetActor(), FXIn, HitLocation.ImpactPoint);
}