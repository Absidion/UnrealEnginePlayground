// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CollectiblesUI.generated.h"

/**
 * 
 */
UCLASS()
class TPPLAYGROUND_API UCollectiblesUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Responsible for adding the ItemToAdd to our UI
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void AddItemToUI();

	//The next item that is going to get added inside our UI
	UPROPERTY(BlueprintReadOnly)
		FText ItemToAdd;
	
public:

	//Adds the name of the item we colelcted inside our UI by using our ADdItem ToUI protected
	void AddItemToUI(FText ItemName);
	
};
