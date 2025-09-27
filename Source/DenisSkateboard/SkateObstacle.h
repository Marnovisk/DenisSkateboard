// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkateObstacle.generated.h"

class UBoxComponent;
class ASkateCharachter;

UCLASS()
class DENISSKATEBOARD_API ASkateObstacle : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SMMesh;

	ASkateCharachter* PlayerChar;
	
public:	
	// Sets default values for this actor's properties
	ASkateObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
