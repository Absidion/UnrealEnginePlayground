// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "FloatingActor.generated.h"


UCLASS()
class FPSPLAYGROUND_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
private:

	//The initial location of the actor
	FVector ActorInitialLocation;

	//The target location which is based on the max bounce height
	FVector TargetLocation;

	//The timeline that will make the actor float
	FTimeline MyTimeline;

	//The function that will handle every tick of the float curve
	UFUNCTION()
	void HandleProgress(float value);

public:	
	// Sets default values for this actor's properties
	AFloatingActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The static mesh of the floating actor
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;

	//The maximum height value that the actor is going to reach.
	UPROPERTY(EditAnywhere, Category = "Timeline")
		float MaxBounceHeight;

	//Curve float reference
	UPROPERTY(EditAnywhere, Category = "Timeline")
		class UCurveFloat* CurveFloat;
	
	
};
