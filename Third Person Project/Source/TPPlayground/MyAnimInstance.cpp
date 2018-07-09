// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyAnimInstance::UpdateAnimationProperties()
{
	//Get the pawn which is affected by our anim instance
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		//Update our falling property - Needed to include "GameFramework/CharacterMovementComponent.h"
		bIsFalling = Pawn->GetMovementComponent()->IsFalling();

		//Update our movement speed
		MovementSpeed = Pawn->GetVelocity().Size();
	}
}
