// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SweepActor.generated.h"

UCLASS()
class FPSPLAYGROUND_API ASweepActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASweepActor();

	//The height of my Sphere starting from the Location of the Actor
	UPROPERTY(EditAnywhere, Category = "Sweep")
		float SphereHeight = 200;

	//The radius of the sphere trace
	UPROPERTY(EditAnywhere, Category = "Sweep")
		float SphereRadius = 500;

	//Sphere segments - used for visualization only
	UPROPERTY(EditAnywhere, Category = "Sweep")
		int32 Segments = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
