// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSPlaygroundHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>

AFPSPlaygroundHUD::AFPSPlaygroundHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AFPSPlaygroundHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	//---------------Radar Logic-----------------

	DrawRadar();
	DrawPlayerInRadar();
	PerformRadarRaycast();
	DrawRaycastedActors();

	//Empty the radar actors, in case the player moves out of range,
	//by doing so, we have always a valid display in our radar.
	RadarActors.Empty();
}

FVector2D AFPSPlaygroundHUD::GetRadarCenterPosition()
{
	//If the canvas is valid, return the center as a 2D Vector
	return (Canvas) ? FVector2D(Canvas->SizeX*RadarStartLocation.X, Canvas->SizeY*RadarStartLocation.Y) : FVector2D(0, 0);
}

void AFPSPlaygroundHUD::DrawRadar()
{
	FVector2D RadarCenter = GetRadarCenterPosition();
	for (float i = 0; i < 360; i += DegreeStep)
	{
		/*We want to draw a circle in order to represent our radar
		In order to do so, we calculate the sin and cos of almost every degree
		It is impossible to calculate each and every possible degree because they ae infinite.
		Lower the degree step in case you need a more accurate circle representation*/

		//We multiply our coordinates by radar size
		//In order to draw a circle with a radius equal to the one we will input through the editor
		float fixedX = FMath::Cos(i) * RadarRadius;
		float fixedY = FMath::Sin(i) * RadarRadius;

		//Actual draw
		DrawLine(RadarCenter.X, RadarCenter.Y, RadarCenter.X + fixedX, RadarCenter.Y + fixedY, FLinearColor::Gray, 1.0f);
	}
}

void AFPSPlaygroundHUD::DrawPlayerInRadar()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	DrawRect(FLinearColor::Blue, RadarCenter.X, RadarCenter.Y, DrawPixelSize, DrawPixelSize);
}

void AFPSPlaygroundHUD::PerformRadarRaycast()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player)
	{

		TArray<FHitResult> HitResults;
		FVector EndLocation = Player->GetActorLocation();
		EndLocation.Z += SphereHeight;

		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Sphere;
		CollisionShape.SetSphere(SphereRadius);

		//Perform the necessary sweep for actors.
		GetWorld()->SweepMultiByChannel(HitResults, Player->GetActorLocation(), EndLocation,
			FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, CollisionShape);

		for (auto It : HitResults)
		{
			AActor* CurrentActor = It.GetActor();
			//In case the actor contains the word "Radar" as a tag, add it to our array
			if (CurrentActor && CurrentActor->ActorHasTag("Radar"))
				RadarActors.Add(CurrentActor);
		}

	}
}

FVector2D AFPSPlaygroundHUD::ConvertWorldLocationToLocal(AActor* ActorToPlace)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player && ActorToPlace)
	{

		//Convert the world location to local, based on the transform of the player
		FVector ActorsLocal3dVector = Player->GetTransform().InverseTransformPosition(ActorToPlace->GetActorLocation());

		//Rotate the vector by 90 degrees, counter-clockwise in order to have a valid rotation in our radar
		ActorsLocal3dVector = FRotator(0.f, -90.f, 0.f).RotateVector(ActorsLocal3dVector);

		//Apply the given distance scale
		ActorsLocal3dVector /= RadarDistanceScale;

		//Return a 2dVector based on the 3d vector we've created above
		return FVector2D(ActorsLocal3dVector);
	}
	return FVector2D(0, 0);
}

void AFPSPlaygroundHUD::DrawRaycastedActors()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	for (auto It : RadarActors)
	{
		FVector2D convertedLocation = ConvertWorldLocationToLocal(It);

		//We want to clamp the location of our actors in order to make sure
		//That we display them inside our radar

		//To do so, i've created the following temporary vector in order to access
		//the GetClmapedtoMaxSize2d function. This function returns a clamped vector(if needed)
		//To match our max length

		FVector tempVector = FVector(convertedLocation.X, convertedLocation.Y, 0.f);

		//subtract the pixel size in order to make teh radar display more accurate
		tempVector = tempVector.GetClampedToMaxSize2D(RadarRadius - DrawPixelSize);

		//Assign the converted X and Y values to the vector we want to display
		convertedLocation.X = tempVector.X;
		convertedLocation.Y = tempVector.Y;

		DrawRect(FLinearColor::Red, RadarCenter.X + convertedLocation.X, RadarCenter.Y + convertedLocation.Y, DrawPixelSize, DrawPixelSize);

	}
}
