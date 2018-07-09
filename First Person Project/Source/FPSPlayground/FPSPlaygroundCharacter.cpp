// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSPlaygroundCharacter.h"
#include "FPSPlaygroundProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"
#include <Components/StaticMeshComponent.h>
#include "Pickup.h"
#include "MyPlayerController.h"
#include <../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFPSPlaygroundCharacter

AFPSPlaygroundCharacter::AFPSPlaygroundCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Grapple"));
	Cable->SetupAttachment(FirstPersonCameraComponent);
	
	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	Hooked = false;
	HookMoveFinished = false;
	HookLocation = FVector(0.0f, 0.0f, 0.0f);
}

void AFPSPlaygroundCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	Cable->SetVisibility(false);
	Cable->EndLocation = FVector(0.f, 0.f, 0.f);

	//Initialize reference for item
	LastItemSeen = nullptr;
	//Initializing our Inventory

	Inventory.SetNum(MAX_INVENTORY_ITEMS);

}

void AFPSPlaygroundCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////Hit contains information about what the raycast hit
	//FHitResult Hit;

	////The Length of the ray in Units
	////For more flexibility you can expose a public variable in the editor
	//float RayLength = 200;

	////The origin of the raycast
	//FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();

	////The endlocation of the raycast
	//FVector EndLocation = StartLocation + (FirstPersonCameraComponent->GetForwardVector() * RayLength);

	////Collision Parameters. The following syntax means that we don't want the trace to be complex
	//FCollisionQueryParams CollisionParameters;

	////Perform the line trace
	////The ECollisionChannel parameter is used in order to determine what we are looking for when performing the raycast
	//ActorLineTraceSingle(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CollisionParameters);

	////DrawDebugLine is used in order to see the raycast we performed
	////The boolean parameter used here means that we want the lines to be persistent so we can see the actual raycast
	////The last parameter is the width of the lines.
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1, 0, 1.f);

	Raycast();

	if (Hooked)
	{
		if (HookMoveFinished)
		{
			MoveGrappledPlayer(UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));

			FVector Newlocation = FMath::VInterpTo(Cable->GetComponentToWorld().GetLocation(), HookLocation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 250.0f);

			Cable->SetWorldLocation(Newlocation);
		}
		else
		{
			HookMoveFinished = MoveRope(UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPSPlaygroundCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &AFPSPlaygroundCharacter::Grapple);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AFPSPlaygroundCharacter::PickupItem);
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &AFPSPlaygroundCharacter::DropEquippedItem);

	FInputActionBinding InventoryBinding;
	//We need this bind to execute on pause state
	InventoryBinding.bExecuteWhenPaused = true;
	InventoryBinding.ActionDelegate.BindDelegate(this, FName("HandleInventoryInput"));
	InventoryBinding.ActionName = FName("Inventory");
	InventoryBinding.KeyEvent = IE_Pressed;

	//Binding the Inventory action
	PlayerInputComponent->AddActionBinding(InventoryBinding);

	//We tell the compiler that we pick the EWaeponType::Knife explicitly
	PlayerInputComponent->BindAction("Knife_Equip", IE_Pressed, this, &AFPSPlaygroundCharacter::EquipWeapon<EWeaponType::Knife>);
	//We tell the compiler that we pick the EWaeponType::Knife explicitly
	PlayerInputComponent->BindAction("Knife_Equip", IE_Pressed, this, &AFPSPlaygroundCharacter::EquipWeapon<EWeaponType::Knife>);
	//We tell the compiler that we pick the EWaeponType::Knife explicitly
	PlayerInputComponent->BindAction("Knife_Equip", IE_Pressed, this, &AFPSPlaygroundCharacter::EquipWeapon<EWeaponType::Knife>);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSPlaygroundCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFPSPlaygroundCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSPlaygroundCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSPlaygroundCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSPlaygroundCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSPlaygroundCharacter::LookUpAtRate);
}
//Created based on the Adding Force to an Actor Tutorial
void AFPSPlaygroundCharacter::MovechosenActor()
{
	//Get the static mesh of the chosen actor
	UStaticMeshComponent* SM = Cast<UStaticMeshComponent>(ActorToMove->GetRootComponent());

	//If the static mesh is vlaid apply the given force
	if (SM)
	{
		//When you want to apply a force you always need to multiple it's value by the mass of the object that the object is applied to
		//SM->AddForce(ForceToAdd*SM->GetMass());

		//When you want to apply an impulse you always need to multiple it's value by the
		//mass of the object that the object is applied to.
		SM->AddImpulse(ForceToAdd*SM->GetMass());
	}
	else
		GLog->Log("RootComponent is not a static mesh!");
}

void AFPSPlaygroundCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AFPSPlaygroundProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AFPSPlaygroundProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AFPSPlaygroundCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFPSPlaygroundCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFPSPlaygroundCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AFPSPlaygroundCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AFPSPlaygroundCharacter::MoveForward(float Value)
{
	if (!Hooked)
	{
		FRotator NewRot;
		NewRot.Roll = 0;
		NewRot.Pitch = 0;
		NewRot.Yaw = GetControlRotation().Yaw;

		FVector ForwardValue = UKismetMathLibrary::GetForwardVector(NewRot);

		AddMovementInput(ForwardValue, Value);

	}
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSPlaygroundCharacter::MoveRight(float Value)
{
	if (!Hooked)
	{
		FRotator NewRot;
		NewRot.Roll = 0;
		NewRot.Pitch = 0;
		NewRot.Yaw = GetControlRotation().Yaw;

		FVector RightValue = UKismetMathLibrary::GetRightVector(NewRot);

		AddMovementInput(RightValue, Value);
	}

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSPlaygroundCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSPlaygroundCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AFPSPlaygroundCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSPlaygroundCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFPSPlaygroundCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFPSPlaygroundCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void AFPSPlaygroundCharacter::EquipWeapon(EWeaponType EWeaponToEquip)
{
	switch (EWeaponToEquip)
	{
	case EWeaponType::Knife:
		{
		GLog->Log("Equipped Knife");
			//more logic here
			break;
		}
	case EWeaponType::Rifle:
		{
		GLog->Log("Equipped Rifle");
			//more logic here
			break;
		}
		case EWeaponType::HandGun:
		{
			GLog->Log("Equipped Handgun");
			//more logic here
			break;
		}
	}
}

void AFPSPlaygroundCharacter::Grapple()
{
	FVector Start;
	FVector End;

	TArray<TEnumAsByte<EObjectTypeQuery>, FDefaultAllocator> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);


	TArray<AActor*> HitObjects;
	TArray<AActor*> ActorsToIgnore;

	FHitResult Hit;

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);

	Start = GetFirstPersonCameraComponent()->GetComponentToWorld().GetLocation();
	End = (GetFirstPersonCameraComponent()->GetForwardVector() * 5000.0f) + GetFirstPersonCameraComponent()->GetComponentToWorld().GetLocation();


	//Hooked = ActorLineTraceSingle(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionParameters);
	Hooked = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	if (Hooked)
	{
		HookLocation = Hit.Location;
	}
	else
	{
		StopGrapple();
		return;
	}

}

void AFPSPlaygroundCharacter::StopGrapple()
{
	Hooked = false;
	HookMoveFinished = false;
	Cable->SetVisibility(false);
	Cable->SetWorldLocation(FVector(0.f, 0.f, 0.f));
}

bool AFPSPlaygroundCharacter::MoveRope(float DeltaTime)
{
	Cable->SetVisibility(true);

	FVector WorldLoc = Cable->GetComponentToWorld().GetLocation();
	if ((WorldLoc - HookLocation).Size() <= 100.0f)
	{
		return true;
	}
	else
	{
		FVector NewLoc = UKismetMathLibrary::VInterpTo(WorldLoc, HookLocation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 35.0f);
		Cable->SetWorldLocation(NewLoc);
		return false;
	}

}

void AFPSPlaygroundCharacter::MoveGrappledPlayer(float DeltaTime)
{
	Cable->SetVisibility(true);
	FVector LaunchVelocity = HookLocation - GetActorLocation();
	float newDelta = UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 250.0f;
	LaunchVelocity = LaunchVelocity * newDelta;

	LaunchCharacter(LaunchVelocity, true, true);
}

void AFPSPlaygroundCharacter::Raycast()
{

	//Calculating start and end location
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndLocation = StartLocation + (FirstPersonCameraComponent->GetForwardVector() * RaycastRange);

	FHitResult RaycastHit;

	//Raycast should ignore the character
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	//Raycast
	GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CQP);

	APickup* Pickup = Cast<APickup>(RaycastHit.GetActor());
	if (LastItemSeen && LastItemSeen != Pickup)
	{
		//If our character sees a different pickup then disable the glowing effect on the previous seen item
		LastItemSeen->SetGlowEffect(false);
	}

	if (Pickup)
	{
		//Enable the glow effect on the current item
		LastItemSeen = Pickup;
		Pickup->SetGlowEffect(true);
	}
	else
		LastItemSeen = nullptr;
}

void AFPSPlaygroundCharacter::PickupItem()
{
	if (LastItemSeen)
	{
		//Find the first available slot
		int32 AvailableSlot = Inventory.Find(nullptr);
		
		if (AvailableSlot != INDEX_NONE)
		{
			//Add the item to the first valid slow we found
			Inventory[AvailableSlot] = LastItemSeen;
			//Destroy the item from the game
			LastItemSeen->Destroy();

		}
		else
			GLog->Log("You can't carry anymore items!");
	}

}

void AFPSPlaygroundCharacter::HandleInventoryInput()
{
	AMyPlayerController* Con = Cast<AMyPlayerController>(GetController());
	if (Con)
	{
		Con->HandleInventoryInput();
	}
}

void AFPSPlaygroundCharacter::SetEquippedItem(UTexture2D * Texture)
{

	if (Texture)
	{
		//For this scenario we make the assumption that
		//every pickup has a unique texture
		//So, in order to set the equpped item we just check every item
		//inside our Inventory. Once we find an item that has the same image as the
		//Texture image we're passing as a parameter we mark that item as CurrentlyEqipped

		for (auto It = Inventory.CreateIterator(); It; It++)
		{

			if ((*It) && (*It)->GetPickupTexture() && (*It)->GetPickupTexture()->HasSameSourceArt(Texture))
			{

				CurrentlyEquippedItem = *It;
				GLog->Log("I've set a new equipped item: " + CurrentlyEquippedItem->GetName());
				break;

			}

		}

	}
	else
		GLog->Log("The Player has clicked an empty inventory slot");

}

void AFPSPlaygroundCharacter::DropEquippedItem()
{
	if (CurrentlyEquippedItem)
	{
		int32 IndexOfItem;
		if (Inventory.Find(CurrentlyEquippedItem, IndexOfItem))
		{
			//The location of the drop
			FVector DropLocation = GetActorLocation() + (GetActorForwardVector() * 200);

			//Making a transform with default rotation and scale. Just setting up the location that was calculated from above
			FTransform Transform;
			Transform.SetLocation(DropLocation);

			//Default actor spawn parameters
			FActorSpawnParameters SpawnParams;

			//Spawn our pickup
			APickup* PickupToSpawn = GetWorld()->SpawnActor<APickup>(CurrentlyEquippedItem->GetClass(), Transform, SpawnParams);

			if (PickupToSpawn)
			{

				//Unreference the item we've just placed
				Inventory[IndexOfItem] = nullptr;

			}

		}

	}
}
