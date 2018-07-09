// Fill out your copyright notice in the Description page of Project Settings.

#include "SweepActor.h"
#include "DrawDebugHelpers.h"


// Sets default values
ASweepActor::ASweepActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASweepActor::BeginPlay()
{
	Super::BeginPlay();

	//TArray is the collection that contains al the HitResults
	TArray<FHitResult> HitResults;

	//The start location of the sphere
	FVector StartLocation = GetActorLocation();

	//The EndLocation of the sphere is equal to the default Location of the actor plus the height we will enter from the editor
	//To extend this functionality, you can replace the height variable with a FVector
	FVector EndLocation = GetActorLocation();
	EndLocation.Z += SphereHeight;

	//Search for static objects only
	ECollisionChannel ECC = ECollisionChannel::ECC_WorldStatic;

	//Declare the Collision Shape, assign a sphere shape and set it's radius
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(SphereRadius);

	//Perform the actual raycast. This method returns true if there is at least 1 hit
	bool bHitSomething = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::FQuat(), ECC, CollisionShape); 

	//If the raycast hit a number of objects, iterate through them and print their name in the console
	if (bHitSomething)
	{
		for (auto It = HitResults.CreateIterator(); It; It++)
		{
			GLog->Log((*It).Actor->GetName());
		}
	}

	//In order to draw the sphere, we can use the DrawDebugSphere function which resides in the DrawDebugHelpers.h
	//This function needs the center of the sphere which in this case is provided by the following equation
	FVector CenterOfSphere = ((EndLocation - StartLocation) / 2) + StartLocation;

	//Draw the sphere in the viewport
	DrawDebugSphere(GetWorld(), CenterOfSphere, CollisionShape.GetSphereRadius(), Segments, FColor::Green, true);

}

// Called every frame
void ASweepActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

