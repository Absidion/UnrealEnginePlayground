// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingActor.h"
#include <Components/StaticMeshComponent.h>


// Sets default values
AFallingActor::AFallingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize a StaticMeshComponent
	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));

	//Enable Physics so the cube will fall
	SM->SetSimulatePhysics(true);

	//You only need to register the OnHit Function
	SM->OnComponentHit.AddDynamic(this, &AFallingActor::OnHit);

}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallingActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GLog->Log(*OtherActor->GetName());
}

void AFallingActor::ReceiveHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	GLog->Log(*OtherActor->GetName());
}

