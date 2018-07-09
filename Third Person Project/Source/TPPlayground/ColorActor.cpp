// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorActor.h"
#include "Components/StaticMeshComponent.h"
#include <Materials/MaterialInstanceDynamic.h>


// Sets default values
AColorActor::AColorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize the static mesh component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
}

// Called when the game starts or when spawned
void AColorActor::BeginPlay()
{
	Super::BeginPlay();

	//Get the first material of the static mesh and turn it into a material instance
	UMaterialInstanceDynamic* DynamicMatInstance = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);

	//Generate random RGB and Alpha values for our Vector Parameter
	FLinearColor RandomColor;
	RandomColor.R = FMath::RandRange(0, 1);
	RandomColor.G = FMath::RandRange(0, 1);
	RandomColor.B = FMath::RandRange(0, 1);
	RandomColor.A = FMath::RandRange(0, 1);

	//If we have a valid dynamic material instance, modify its parameters
	if (DynamicMatInstance)
	{
		DynamicMatInstance->SetVectorParameterValue(FName("ColorParam"), RandomColor);
		DynamicMatInstance->SetScalarParameterValue(FName("MetalParam"), FMath::RandRange(0, 1));
	}

	
}

// Called every frame
void AColorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

