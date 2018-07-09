// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlotWidget.h"
#include "Pickup.h"
#include "FPSPlaygroundCharacter.h"
#include "Kismet/GameplayStatics.h"

void UInventorySlotWidget::SetEquippedItem()
{
	AFPSPlaygroundCharacter* Char = Cast<AFPSPlaygroundCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Char)
	{
		Char->SetEquippedItem(ItemTexture);
	}
}

void UInventorySlotWidget::SetItemTexture(APickup* Item)
{
	//If the item is valid, update the widget's texture.
	//If not, assign a null ptr to it so the widget won't broadcast wrong info to the player
	(Item) ? ItemTexture = Item->GetPickupTexture() : ItemTexture = nullptr;
}
