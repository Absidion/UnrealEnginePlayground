// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSPlaygroundCharacter.generated.h"

#define MAX_INVENTORY_ITEMS 4

enum class EWeaponType : uint8
{
	Knife,
	HandGun,
	Rifle
};

class UInputComponent;

UCLASS(config=Game)
class AFPSPlaygroundCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

public:
	AFPSPlaygroundCharacter();

protected:
	virtual void BeginPlay();

	void Tick(float DeltaTime) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFPSPlaygroundProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	//Adding Force to an Actor Tutorial Area

	//The actor which will be selected from the Viewport
	UPROPERTY(EditAnywhere, Category = "Physics")
		AActor* ActorToMove;

	UPROPERTY(VisibleAnywhere, Category = "Grapple")
	class UCableComponent* Cable;

	//The force that we're going to apply to the chosen actor with a default value
	UPROPERTY(EditAnywhere, Category = "Physics")
		FVector ForceToAdd = FVector(0, 0, 5000);

	//The function that applies the above force to the chosen actor
	UFUNCTION(BlueprintCallable, Category = "Physics")
		void MovechosenActor();

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

	bool Hooked;
	bool HookMoveFinished;
	FVector HookLocation;

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	//This function handles the weapon equipment
	void EquipWeapon(EWeaponType EWeaponToEquip);

	//Templated version of EquipWeapon which calls the above function
	template<EWeaponType EWeaponToEquip>
	void EquipWeapon() { EquipWeapon(EWeaponToEquip); }

	void Grapple();
	void StopGrapple();

	bool MoveRope(float DeltaTime);
	void MoveGrappledPlayer(float DeltaTime);

private:
	//Raycasts in front of the character to find usable items
	void Raycast();

	//Reference to the last seen pickuip item. Nullptr if none
	class APickup* LastItemSeen;

	//Reference to the currently equipped item
	class APickup* CurrentlyEquippedItem;

public:

	//The range of the raycast
	UPROPERTY(EditAnywhere)
	float RaycastRange = 250.0f;

	//Handles the Pickup Input
	UFUNCTION()
		void PickupItem();
	
	//The actual Inventory
	UPROPERTY(VisibleAnywhere)
		TArray<APickup*> Inventory;

	//Handles the inventory input by sending information to the controller
	UFUNCTION()
		void HandleInventoryInput();

	//Sets a new equipped item based on the given texture
	void SetEquippedItem(UTexture2D* Texture);

	//Drops the currently equipped item
	UFUNCTION()
		void DropEquippedItem();

};

