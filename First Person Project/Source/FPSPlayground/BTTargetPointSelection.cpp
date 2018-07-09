//// Fill out your copyright notice in the Description page of Project Settings.
//
//#include "BTTargetPointSelection.h"
//#include "BotTargetPoint.h"
//#include "MyAIController.h"
//#include "BehaviorTree/BlackboardComponent.h"
//
//
//
//EBTNodeResult::Type UBTTargetPointSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	AMyAIController* AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());
//	
//	//If the controller is valid:
//	//1) Get the blackboard component and the current point of the bot
//	//2) Search for the next point, which will be different from the current point
//
//	if (AICon)
//	{
//		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();
//		ABotTargetPoint* CurrentPoint = Cast<ABotTargetPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));
//
//		TArray<AActor*> AvailableTargetPoints = AICon->GetAvailableTargetPoints();
//
//		//This variable will contain a random index in order to determine the next possible point
//		int32 RandomIndex;
//
//		//Here, we store the possible next target point
//		ABotTargetPoint* NextTargetPoint = nullptr;
//
//		//Find a next point which is different from the current one
//		do 
//		{
//			RandomIndex = FMath::RandRange(0, AvailableTargetPoints.Num() - 1);
//			//Remembers that the array provided by the controller function contains AActor* objects so we need to cast.
//			NextTargetPoint = Cast<ABotTargetPoint>(AvailableTargetPoints[RandomIndex]);
//		} while (CurrentPoint == NextTargetPoint);
//
//		//Update the next location in the Blackboard so the bot can move to the next Blackboard Value
//		BlackboardComp->SetValueAsObject("LocationToGo", NextTargetPoint);
//
//		//At this point, the task has been successfully completed
//		return EBTNodeResult::Succeeded;
//	}
//	return EBTNodeResult::Failed;
//}
