// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class FPSPLAYGROUND_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//The component which is used for the "seeing" sense of the AI
	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;

private:

	UFUNCTION()
		void OnSeenPlayer(APawn* Pawn);
	
	
};
