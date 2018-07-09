// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorActor.generated.h"

UCLASS()
class TPPLAYGROUND_API AColorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The static mesh of the actor
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
