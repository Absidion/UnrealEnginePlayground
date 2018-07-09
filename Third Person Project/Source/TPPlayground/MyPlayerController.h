// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPPLAYGROUND_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	//Our UI Reference
	class UCollectiblesUI* CollectiblesWidget;

protected:
	//Property which is pointing to our Widget Blueprint in order to instantiate it using c++
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCollectiblesUI> CollectiblesWidgetBP;

public:
	//Executes when the controller possess a pawn
	virtual void Possess(APawn* InPawn) override;

	//Updates the UI based on the item that the player has collected
	void UpdateCollectedItems(FText ItemName);
	
	
};
