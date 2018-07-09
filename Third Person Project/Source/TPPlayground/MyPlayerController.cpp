// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "CollectiblesUI.h"
#include "TPPlaygroundCharacter.h"

void AMyPlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	if (CollectiblesWidgetBP)
	{
		//Creating our widget and adding it to our viewport
		CollectiblesWidget = CreateWidget<UCollectiblesUI>(this, CollectiblesWidgetBP);

		CollectiblesWidget->AddToViewport();
	}
}

void AMyPlayerController::UpdateCollectedItems(FText ItemName)
{
	ATPPlaygroundCharacter* Char = Cast<ATPPlaygroundCharacter>(GetPawn());

	if (Char)
	{
		//Updating our UI with the new collected item
		CollectiblesWidget->AddItemToUI(ItemName);
	}
}

