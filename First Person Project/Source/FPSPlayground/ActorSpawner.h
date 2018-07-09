// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsefulActor.h"
#include "ActorSpawner.generated.h"

UCLASS()
class FPSPLAYGROUND_API AActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Blueprint Reference of UsefulActor class
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		TSubclassOf<AUsefulActor> UsefulActorBP;

	//Delay after the Blueprint of the UsefulActor will get spawned
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
		float TimeToSpawn = 2.0f;

	//Spawns the UsefulActor Blueprint
	UFUNCTION()
		void SpawnUsefulActor();
	
};
