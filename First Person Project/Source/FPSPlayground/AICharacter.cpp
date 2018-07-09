// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include <Perception/PawnSensingComponent.h>


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the pawn sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

//Set the periperhal vision angle to 90 degrees
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	//Register the function that is going to fire when the character sees a Pawn
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacter::OnSeenPlayer);
	}
	
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::OnSeenPlayer(APawn* Pawn)
{
	AMyAIController* AIController = Cast<AMyAIController>(GetController());
	//Set the seen target on the blackboard
	if (AIController)
	{
		GLog->Log("Oh hello there");
		AIController->SetSeenTarget(Pawn);
	}
}

