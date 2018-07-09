// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "TPPlaygroundCharacter.generated.h"

UCLASS(config=Game)
class ATPPlaygroundCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ATPPlaygroundCharacter();

	//The function that is going to play the sound and report it to our game
	UFUNCTION(BlueprintCallable, Category = "AI")
		void ReportNoise(class USoundBase* SoundToPlay, float Volume);

	//A Pawn Noise emitter component which is used in order to emit the sounds to nearby AIs
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPawnNoiseEmitterComponent* PawnNoiseEmitterComp;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Adds the items to the collectibles UI
	void AddItemToUI(FText ItemName);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);



protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//This function will search which function to execute
	UFUNCTION(BlueprintCallable, Category = "Reflection")
		void ExecuteFunction(FString FuntionToExecute);

	UFUNCTION()
		void DoSomething();

private:

	//A simple function which prints its parameters
	UFUNCTION()
		void OneParamFunction(int32 Param);

	//A simple function which prints its parameters
	UFUNCTION()
		void TwoParamsFunction(int32 IntParam, float FloatParam);

	//A function that changes the parameters and replaces a hardcoded action bind
	void ChangeParameters();

protected:

	//The parameter which we will pass into the OneParamFunction
	UPROPERTY(EditDefaultsOnly, Category = "InputsWithParameters")
		int32 FirstIntParam;

	//The int parameter that will get passed into the TwoParamsFunction
	UPROPERTY(EditDefaultsOnly, Category = "InputsWithParameters")
		int32 SecondIntParam;

	//The float parameter that will get passed into the TwoParamsFunction
	UPROPERTY(EditDefaultsOnly, Category = "InputsWithParameters")
		float FloatParam;

	//Multithreading Tutorial
	
	//Calculates prime numbers in the game thread
	UFUNCTION(BlueprintCallable, Category = "MultiThreading")
		void CalculatePrimeNumbers();

	//Calculates prim numbers in a background thread
	UFUNCTION(BlueprintCallable, Category = "MultiThreading")
		void CalculatePrimeNumbersAsync();

	//The max prime number
	UPROPERTY(EditAnywhere, Category = "MultiThreading")
		int32 MaxPrime;

};

namespace ThreadingTest
{
	static void CalculatePrimeNumbers(int32 UpperLimit)
	{
		//Calculating the prime numbers...
		for (int32 i = 1; i <= UpperLimit; i++)
		{
			bool isPrime = true;

			for (int32 j = 2; j <= i / 2; j++)
			{
				if (FMath::Fmod(i, j) == 0)
				{
					isPrime = false;
					break;
				}
			}

			if (isPrime) GLog->Log("Prime number #" + FString::FromInt(i) + ": " + FString::FromInt(i));

		}
	}
}

//PrimeCalculateAsyncTask is the name of our Task
//FNonAbandonableTask is the name of the class for tasks
class PrimeCalculationAsyncTask : public FNonAbandonableTask
{
	int32 MaxPrime;
public:
	//default constructor
	PrimeCalculationAsyncTask(int32 MaxPrime)
	{
		this->MaxPrime = MaxPrime;
	}

	//This function is needed from the API of the engine.
	//My guess is that it provides necessary information
	//about the thread that we occupy and the progress of our task
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(PrimeCalculationAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	//This function is executed when we tell our task to execute
	void DoWork()
	{
		ThreadingTest::CalculatePrimeNumbers(MaxPrime);

		GLog->Log("--------------------------------------------------------------------");
		GLog->Log("End of prime numbers calculation on game thread");
		GLog->Log("--------------------------------------------------------------------");
	}

};

