// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class FPSPLAYGROUND_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The static mesh of the Pickup
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* PickupSM;

	//The texture of the item incase we want to add it in the secrets or inventory
	UPROPERTY(EditAnywhere, Category = "PickupProperties")
		UTexture2D* PickupTexture;

	//The name of the item
	UPROPERTY(EditAnywhere, Category = "PickupProperties")
		FString ItemName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Enables and Disables the glow effect on the pickup
	void SetGlowEffect(bool Status);

	//Returns the texture of our pickup
	FORCEINLINE UTexture2D* GetPickupTexture() { return PickupTexture; }



	
	
};
