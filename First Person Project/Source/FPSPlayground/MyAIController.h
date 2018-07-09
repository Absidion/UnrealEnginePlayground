// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSPLAYGROUND_API AMyAIController : public AAIController
{
	GENERATED_BODY()
private:
	//Behavior Tree comp ref
	class UBehaviorTreeComponent* BehaviorComp;

	//--------Blackboard---------//

	//Blackboard comp ref
	class UBlackboardComponent* BlackboardComp;

	//Blackboard Keys
	//UPROPERTY(EditDefaultsOnly, Category = "AI")
		//FName LocationToGoKey;



	//--------Blackboard---------//


	//TArray<AActor*> BotTargetPoints;

	//Possess is executed when the character we want to control is spawned.
	//Inside this function, we initialize the blackboard and start the behavior tree


	
public:
	//Constructor
	AMyAIController();

	//Executes right when the controller possesses a pawn
	virtual void Possess(APawn* Pawn) override;

	//Sets the sensed target in the blackboard
	void SetSeenTarget(APawn* Pawn);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName BlackboardKey = "Target";

	//FORCEINLINE class UBlackboardComponent* GEtBlackboardComp() const { return BlackboardComp; }

	//FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }
	
};
