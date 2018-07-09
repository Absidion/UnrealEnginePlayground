// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerBox.h"


void ACustomTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	//Register the enter and exit overlaps to fire

	OnActorBeginOverlap.AddDynamic(this, &ACustomTriggerBox::OnTriggerEnter);
	OnActorEndOverlap.AddDynamic(this, &ACustomTriggerBox::OnTriggerExit);
}

void ACustomTriggerBox::OnTriggerEnter(AActor* OverlapedActor, AActor* OtherActor)
{
	GLog->Log("Begin Overlap has fired");
}

void ACustomTriggerBox::OnTriggerExit(AActor* OverlapedActor, AActor* OtherActor)
{
	GLog->Log("End Overlap has fired");
}
