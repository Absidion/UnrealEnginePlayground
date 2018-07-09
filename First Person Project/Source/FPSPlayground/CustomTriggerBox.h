// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CustomTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class FPSPLAYGROUND_API ACustomTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:

	//Overriding the BeginPlay function
	virtual void BeginPlay() override;

	//This function is called when an actor enters the Trigger - 4.12 Version
	UFUNCTION()
	void OnTriggerEnter(AActor* OverlapedActor, AActor* OtherActor);

	UFUNCTION()
	void OnTriggerExit(AActor* OverlapedActor, AActor* OtherActor);
	
	
	
	
};
