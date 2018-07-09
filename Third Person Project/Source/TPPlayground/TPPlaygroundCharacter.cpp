// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TPPlaygroundCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// ATPPlaygroundCharacter

ATPPlaygroundCharacter::ATPPlaygroundCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ATPPlaygroundCharacter::ReportNoise(USoundBase* SoundToPlay, float Volume)
{
	if (SoundToPlay)
	{
		//Play the actual sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), Volume);

		//Report that we've played a sound with a certain volume in a specific location
		MakeNoise(Volume, this, GetActorLocation());
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPPlaygroundCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPPlaygroundCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPPlaygroundCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATPPlaygroundCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATPPlaygroundCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATPPlaygroundCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATPPlaygroundCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATPPlaygroundCharacter::OnResetVR);

	//First Param Action binding...
	
	//Declaring an action binding
	FInputActionBinding ActionBindingOneParam;

	//Specifying the assocaited action name and the key event
	ActionBindingOneParam.ActionName = FName("OneParamInput");
	ActionBindingOneParam.KeyEvent = IE_Pressed;

	//Creating a handler which binds to the given function with a fixed int parameter
	FInputActionHandlerSignature OneParamActionHandler;

	//Binding the function named OneParamFunction to our Handler
	//The first parameter (this) means that the handler will search the given function insider the current class
	OneParamActionHandler.BindUFunction(this, FName("OneParamFunction"), FirstIntParam);

	//Associating our action binding with our new delegate
	ActionBindingOneParam.ActionDelegate = OneParamActionHandler;

	//Performing the action binding...
	PlayerInputComponent->AddActionBinding(ActionBindingOneParam);

	//Second Param - Identical code to the first param action bind but with a different function and parameters

	FInputActionBinding ActionBindingTwoParams;

	ActionBindingTwoParams.ActionName = FName("TwoParamsInput");
	ActionBindingTwoParams.KeyEvent = IE_Pressed;

	FInputActionHandlerSignature TwoParamsActionHandler;
	TwoParamsActionHandler.BindUFunction(this, FName("TwoParamsFunction"), SecondIntParam, FloatParam);

	ActionBindingTwoParams.ActionDelegate = TwoParamsActionHandler;

	PlayerInputComponent->AddActionBinding(ActionBindingTwoParams);

	//Binding the change parameters function
	InputComponent->BindAction("ChangeParameters", IE_Pressed, this, &ATPPlaygroundCharacter::ChangeParameters);
}


void ATPPlaygroundCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATPPlaygroundCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATPPlaygroundCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATPPlaygroundCharacter::AddItemToUI(FText ItemName)
{
	AMyPlayerController* Con = Cast<AMyPlayerController>(GetController());

	if (Con)
	{
		Con->UpdateCollectedItems(ItemName);
	}

}

void ATPPlaygroundCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPPlaygroundCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATPPlaygroundCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATPPlaygroundCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATPPlaygroundCharacter::ExecuteFunction(FString FunctionToExecute)
{
	//FindFunction will return a pointer to a UFunction based on a
	//Given FName. We use an asterisk before our FString in order to convert the FString variable to FName
	UFunction* function = FindFunction(*FunctionToExecute);

	//When you're using a pointer - make sure to check if it's valid first!
	if (function)
	{
		//The following point is a void pointer,
		//this means that it can point to anything - from raw memory to al the known types
		void* locals = nullptr;

		//In order to execute our function we need to reserve a chunk of memory in
		//The execution stack for it.
		FFrame* frame = new FFrame(this, function, locals);

		//Source code doesn't explain what locals pointer is used for

		//Actual call of our UFunction
		CallFunction(*frame, locals, function);

		//Delete our pointer to avoid memory leaks;
		delete frame;
	}
}

void ATPPlaygroundCharacter::DoSomething()
{
	GLog->Log("I'm doing something!");
}

void ATPPlaygroundCharacter::OneParamFunction(int32 Param)
{
	//printing the given parameter
	GLog->Log("The parameter you've entered: " + FString::FromInt(Param));
}

void ATPPlaygroundCharacter::TwoParamsFunction(int32 IntParam, float FloatParam)
{
	//printing the given parameters
	GLog->Log("Input with two parameters. First Param: " + FString::FromInt(IntParam) + ". Second Param: " + FString::SanitizeFloat(FloatParam));
}

void ATPPlaygroundCharacter::ChangeParameters()
{
	//Choosing a different random param!
	SecondIntParam = FMath::Rand();

	FloatParam = FMath::RandRange(-1000.f, 1000.f);

	GLog->Log("Changed Params: " + FString::FromInt(SecondIntParam) + " " + FString::SanitizeFloat(FloatParam));

	//Search all the action bindings until we find the binding with the two parameters
	for (int32 i = 0; i < InputComponent->GetNumActionBindings() - 1; i++)
	{
	
		if (InputComponent->GetActionBinding(i).ActionName.IsEqual("TwoParamsInput"))
		{

			//Declaring a new binding with the same action and key event as the TwoParamsInput
			FInputActionBinding NewActionBinding;

			NewActionBinding.ActionName = FName("TwoParamsInput");
			NewActionBinding.KeyEvent = IE_Pressed;

			//Creating an identical handler with the same bind as before
			FInputActionHandlerSignature NewDelegate;
			//However, this bind contains our new values!
			NewDelegate.BindUFunction(this, FName("TwoParamsFunction"), SecondIntParam, FloatParam);

			//Associating our handler with our action binding
			NewActionBinding.ActionDelegate = NewDelegate;

			//The getActionBinding function returns the action binding by reference
			//This means that in the following line we replace the old binding (which contains old values)
			//With a new binding (which contains the updated values)
			InputComponent->GetActionBinding(i) = NewActionBinding;

			GLog->Log("Changed the action binding");
		}
	}
}

void ATPPlaygroundCharacter::CalculatePrimeNumbers()
{
	//Performing the prime numbers calculations in the game thread...
	ThreadingTest::CalculatePrimeNumbers(MaxPrime);

	GLog->Log("--------------------------------------------------------------------");
	GLog->Log("End of prime numbers calculation on game thread");
	GLog->Log("--------------------------------------------------------------------");
}

void ATPPlaygroundCharacter::CalculatePrimeNumbersAsync()
{
	/*Create a new Task and pass as a parameter our MaxPrime
	Then, tell that Task to execute in the background.
	
	The FAutoDeleteAsyncTask will make sure to delete the task when it's finished.
	
	Multithreading requires cautious handle of the available threads, in order to avoid
	race conditions and strange bugs that are not easy to solve.
	
	Fortunately, UE4 contains a class(FAutoDeleteAsyncTask) which handles everything by itself
	and the programmer is able to perform async operations without any real effort.*/

	(new FAutoDeleteAsyncTask<PrimeCalculationAsyncTask>(MaxPrime))->StartBackgroundTask();
}
