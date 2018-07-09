// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectiblesUI.h"

void UCollectiblesUI::AddItemToUI(FText ItemName)
{
	ItemToAdd = ItemName;
	AddItemToUI();
}


