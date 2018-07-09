// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSPlaygroundHUD.generated.h"

UCLASS()
class AFPSPlaygroundHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFPSPlaygroundHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	void PerformRadarRaycast();

protected:

	//The start location of our Radar
	UPROPERTY(EditAnywhere, Category = "Radar")
		FVector2D RadarStartLocation = FVector2D(0.9f, 0.2f);

	//The radius of our Radar
	UPROPERTY(EditAnywhere, Category = "Radar")
		float RadarRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Radar")
		float DegreeStep = 0.25f;

	//The pixel size of the drawable Radar actors
	UPROPERTY(EditAnywhere, Category = "Radar")
		float DrawPixelSize = 5.0f;

	//Sphere Height and radius for our raycast
	UPROPERTY(EditAnywhere, Category = "Radar")
		float SphereHeight = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Radar")
		float SphereRadius = 2750.0f;

	//The distance scale of the radar actors
	UPROPERTY(EditAnywhere, Category = "Radar")
		float RadarDistanceScale = 25.0f;

	//Converts the given actors location to local (Based on our character)
	FVector2D ConvertWorldLocationToLocal(AActor* ActorToPlace);

	//Draws the raycasted actors in our radar
	void DrawRaycastedActors();

	//Holds a reference to every actor we are currently drawing in our radar
	TArray<AActor*> RadarActors;

	//Returns the center of the Radar as a 2D Vector
	FVector2D GetRadarCenterPosition();

	//Draws the Radar
	void DrawRadar();

	//Draws the player in the Radar
	void DrawPlayerInRadar();




};

