// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyDelegates.generated.h"

UCLASS()
class FPSPLAYGROUND_API AMyDelegates : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDelegates();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Single-cast Delegate declaration. No Parameters.
	DECLARE_DELEGATE(MyDelegate);

	//Single-cast delegate declaration. One int32 Parameter.
	DECLARE_DELEGATE_OneParam(MyIntDelegate, int32);

	//Multi-cast delegate declaration. One int32 Parameter.
	DECLARE_MULTICAST_DELEGATE_OneParam(MyIntMulticastDelegate, int32);

	//Function with one int32 parameter
	UFUNCTION()
		void IntFunction(int32 x);

	//Function with one int32 parameter
	UFUNCTION()
		void SecondIntFunction(int32 x);

	//Function with one int32 parameter
	UFUNCTION()
		void ThirdIntFunction(int32 x);

	//Function with no parameters
	UFUNCTION()
		void SomeFunction();

	
	
};
