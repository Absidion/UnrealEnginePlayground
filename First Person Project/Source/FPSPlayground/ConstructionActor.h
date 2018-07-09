// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructionActor.generated.h"

UCLASS()
class FPSPLAYGROUND_API AConstructionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConstructionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The static mesh of our actor
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* SM;

	//The random location of our component will get generated based on a given X and Y from within our editor

	//The max X of the location vector that the static mesh will be spawned
	UPROPERTY(EditAnywhere, Category = "Construction")
		float XThreshold;

	//The max Y of the location vector that the static mesh will be spawned
	UPROPERTY(EditAnywhere, Category = "Construction")
		float YThreshold;

	//The number of meshes/components that will get spawned
	UPROPERTY(EditAnywhere, Category = "Construction")
		int32 NumToSpawn;

	//A random seed. Editing this, the editor will generate new random locations
	UPROPERTY(EditAnywhere, Category = "Construction")
		int32 RandomSeed;

	//An array containing references of the spawned components
	//This will be used in order to delete old components in case we decide to tinker with the NumToSpawn parameter
	TArray<class UStaticMeshComponent*> SMArray;
	
};
