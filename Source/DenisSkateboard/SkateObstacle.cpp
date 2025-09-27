// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateObstacle.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "SkateCharachter.h"

// Sets default values
ASkateObstacle::ASkateObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//--CapsuleCollision Setup for Actor
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	BoxCollision->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = BoxCollision;

	//--SkeltalMesh Setup for Actor
	SMMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMMesh"));
	SMMesh->SetupAttachment(RootComponent);
	SMMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	SMMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	//SMMesh->SetSkeletalMesh(StaticMeshAsset.Object);

}

// Called when the game starts or when spawned
void ASkateObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkateObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + FVector(0.0f, 0.0f, 200.0f); // Trace distance

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this); // Ignore self
    TraceParams.AddIgnoredActor(GetAttachParentActor());

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Pawn,
        TraceParams
    );

    if (bHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *HitResult.GetActor()->GetName());

        if (ASkateCharachter* Car = Cast<ASkateCharachter>(HitResult.GetActor()))
        {
            UE_LOG(LogTemp, Log, TEXT("LineTrace hit CarCore: %s"), *Car->GetName());
            Car->IncreaseScore();
        }

        // Debug visualization
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f);
        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Yellow, false, 0.1f);
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.1f);
    }

}

