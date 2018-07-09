// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "UPlayerStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XpToLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
};