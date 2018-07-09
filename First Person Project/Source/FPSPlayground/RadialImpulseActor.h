// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialImpulseActor.generated.h"

UCLASS()
class FPSPLAYGROUND_API ARadialImpulseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARadialImpulseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Radius of the impulse
	UPROPERTY(EditAnywhere)
		float radius;

	//Strength of the impulse
	UPROPERTY(EditAnywhere)
		float strength;

	//A dynamic array of nearby actors
	UPROPERTY(EditAnywhere)
		TArray<AActor*> NearbyActors;

	
	
};
