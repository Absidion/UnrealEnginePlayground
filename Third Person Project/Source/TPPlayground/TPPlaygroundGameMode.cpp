// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TPPlaygroundGameMode.h"
#include "TPPlaygroundCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPPlaygroundGameMode::ATPPlaygroundGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATPPlaygroundGameMode::DoSomething()
{
	GLog->Log("Doing something");
}

void ATPPlaygroundGameMode::DoSomethingElse(float param)
{
	GLog->Log("Param: " + FString::SanitizeFloat(param));
}

void ATPPlaygroundGameMode::DoubleParamFunction(float param1, int32 param2)
{
	GLog->Log("Param1: " + FString::SanitizeFloat(param1) + " - Param2: " + FString::FromInt(param2));
}
