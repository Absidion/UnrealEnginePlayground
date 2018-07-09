// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorSpawner.h"

// Sets default values
AActorSpawner::AActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle OutHandle;

	//Will call SpawnUsefulActor after the specified time
	GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &AActorSpawner::SpawnUsefulActor, TimeToSpawn);
	
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UsefulActorBP->GetDefaultObject<AUsefulActor>()->DoSomething(); //This line gets the PARENT class and calls a function within it

}

void AActorSpawner::SpawnUsefulActor()
{
	//If the usefulactorbp is valid
	if (UsefulActorBP)
	{
		//Spawn Parameters for the current spawn.
		//We can use this for a number of options like disable collision or adjust the spawn position
		//If a collision is happening in the spawn point etc..
		FActorSpawnParameters SpawnParams;

		//Actual Spawn. The following function returns a reference to the spawned actor
		AUsefulActor* ActorRef = GetWorld()->SpawnActor<AUsefulActor>(UsefulActorBP, GetTransform(), SpawnParams);

		GLog->Log("Spawned the Useful Actor.");
	}
}

