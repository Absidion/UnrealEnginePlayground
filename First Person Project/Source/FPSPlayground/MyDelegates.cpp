// Fill out your copyright notice in the Description page of Project Settings.

#include "MyDelegates.h"


// Sets default values
AMyDelegates::AMyDelegates()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyDelegates::BeginPlay()
{
	Super::BeginPlay();

	//Declaring a delegate of MyDelegate type
	MyDelegate MyDel;
	//Binding a UFUNCTION to MyDel - this will not call the function just yet.
	MyDel.BindUFunction(this, FName("SomeFunction"));

	//Calling the bound function of the delegate
	MyDel.Execute();

	//Declaring a delegate of MyIntDelegate type
	MyIntDelegate IntDelegate;

	//Binding two UFUNCTIOs to IntDelegate - This will not call any function just yet
	IntDelegate.BindUFunction(this, FName("IntFunction"));
	IntDelegate.BindUFunction(this, FName("SecondIntFunction"));

	//Since the IntDelegate is a single-cast delegate it will only contain up to one function
	//In this case, the IntDelegate contains the last bound function
	IntDelegate.Execute(50);

	//Declaring a delegate of MyIntMulticastDelegate type
	MyIntMulticastDelegate Del;

	//Adding three UFUNCTIONs to the delegate - this will not call any function just yet.
	Del.AddUFunction(this, FName("IntFunction"));
	Del.AddUFunction(this, FName("SecondIntFunction"));
	Del.AddUFunction(this, FName("ThirdIntFunction"));

	//Calling all the bound functions with a value of 10
	Del.Broadcast(10);

	
}

// Called every frame
void AMyDelegates::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDelegates::IntFunction(int32 x)
{
	GLog->Log("Output from IntFunction: " + FString::FromInt(x));
}

void AMyDelegates::SecondIntFunction(int32 x)
{
	GLog->Log("Output from SecondIntFunction: " + FString::FromInt(x * 2));
}

void AMyDelegates::ThirdIntFunction(int32 x)
{
	//x to square
	float power = FMath::Pow(x, 2);
	GLog->Log("Third Int Function: " + FString::SanitizeFloat(power));
}

void AMyDelegates::SomeFunction()
{
	GLog->Log("Some function log");
}

