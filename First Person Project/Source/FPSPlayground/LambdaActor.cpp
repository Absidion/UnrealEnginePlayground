// Fill out your copyright notice in the Description page of Project Settings.

#include "LambdaActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALambdaActor::ALambdaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALambdaActor::BeginPlay()
{
	Super::BeginPlay();

	//Declaring an Array of Actors
	TArray<AActor*> ActorsArray;

	//Declaring a delegate with one int32 parameter
	DECLARE_DELEGATE_OneParam(MyUsefulDelegate, int32);

	//The following functions populates the ActorsArray with all the actors which reside inside our current level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorsArray);

	//Declaring a MyUsefulDelegate
	MyUsefulDelegate Del;

	//Binding a Lambda to it
	//In this lambda we pass the ActorsArray by value since we won't make any changes or want any changes reflected outside the lambda expression.
	//If we don't pass the ActorsArray in the capturelist then we won't be able to have access to it!
	Del.BindLambda([ActorsArray](int32 ActorIndex) 
	{
		//Print the name of the actor which has an index equal to the one we provided (ActorIndex)
		//Make sure we provided a valid index for our array
		if (ActorsArray.IsValidIndex(ActorIndex))
			GLog->Log("Actor with given index: " + ActorsArray[ActorIndex]->GetName());
		else
			GLog->Log("You've entered an invalid index. That's unfortunate :(");
	});

	//Show me the 16th actor of the Array - Don't forget that TArray is zero-based!
	Del.ExecuteIfBound(15);

}

// Called every frame
void ALambdaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

