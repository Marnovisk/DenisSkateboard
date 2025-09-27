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

    

    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Game/Deko_MatrixDemo/City/Meshes/SM_BLDG_Prop_SB_Awning_B01_N1.SM_BLDG_Prop_SB_Awning_B01_N1"));
	
	//--SkeltalMesh Setup for Actor
	SMMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMMesh"));
	SMMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	SMMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    RootComponent = SMMesh;
    if (StaticMeshAsset.Succeeded())
    {
        SMMesh->SetStaticMesh(StaticMeshAsset.Object);
    }

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

    FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
    FVector End = Start + FVector(0.0f, 0.0f, 200.0f); // Trace distance

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);
    TraceParams.AddIgnoredActor(GetAttachParentActor());

    float SphereRadius = 50.0f;

    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(SphereRadius),
        TraceParams
    );

    if (bHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *HitResult.GetActor()->GetName());

        if (ASkateCharachter* Char = Cast<ASkateCharachter>(HitResult.GetActor()))
        {
            if (PlayerChar != Char)
            {
                UE_LOG(LogTemp, Log, TEXT("SphereSweep hit SkateChar: %s"), *Char->GetName());
                Char->IncreaseScore();
                PlayerChar = Char;
            }
        }
    }
}

