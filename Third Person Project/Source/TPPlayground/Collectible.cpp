// Fill out your copyright notice in the Description page of Project Settings.

#include "Collectible.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "TPPlaygroundCharacter.h"


// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creating our components
	SM = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM"));
	RotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(FName("RotatingMoveComp"));

}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACollectible::Overlap);
	
}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectible::Overlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ATPPlaygroundCharacter* Char = Cast<ATPPlaygroundCharacter>(OtherActor);
	if (Char)
	{
		//Get the name of our item
		FText Text = FText::FromString(GetName());
		//Add the item to our UI
		Char->AddItemToUI(Text);
		Destroy();
	}
}

