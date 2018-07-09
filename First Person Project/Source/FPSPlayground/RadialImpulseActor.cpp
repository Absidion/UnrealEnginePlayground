// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialImpulseActor.h"
#include <Components/StaticMeshComponent.h>


// Sets default values
ARadialImpulseActor::ARadialImpulseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARadialImpulseActor::BeginPlay()
{
	Super::BeginPlay();

	//Get all the nearby actors
	for(auto It = NearbyActors.CreateIterator(); It; It++)
	{
		//Get the static mesh component for each actor
		UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*It)->GetRootComponent());

		//If the component is valid, apply a radial impulse from the location of this actor
		//Having as radius and strength the values from the Editor
		//The RIF_Linear impulse fall off parameter means that we want the impulse to fall off to zero when the max distance is reached.
		//The last parameter means that we want to ignore the mass of each static mesh

		if (SM)
		{
			SM->AddRadialImpulse(GetActorLocation(), radius, strength, ERadialImpulseFalloff::RIF_Linear, true);
		}
	}
	
}

// Called every frame
void ARadialImpulseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

