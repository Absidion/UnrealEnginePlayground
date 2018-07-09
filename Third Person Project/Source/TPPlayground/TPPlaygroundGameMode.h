// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPPlaygroundGameMode.generated.h"

UCLASS(minimalapi)
class ATPPlaygroundGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPPlaygroundGameMode();

	//Function with no Parameters
	UFUNCTION(Exec, Category = "ExecFunctions")
		void DoSomething();

	//Function with one Parameter
	UFUNCTION(Exec, Category = "ExecFunctions")
		void DoSomethingElse(float param);

	//Function with two parameters
	UFUNCTION(Exec, Category = "ExecFunctions")
		void DoubleParamFunction(float param1, int32 param2);
};



