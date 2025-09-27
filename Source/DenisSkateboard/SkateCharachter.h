// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkateCharachter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCapsuleComponent;
class USkeletalMeshComponent;
class UAnimationAsset;
struct FInputActionValue;

UCLASS()
class DENISSKATEBOARD_API ASkateCharachter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SMSkateMesh;

	//--Camera Properties

	//Camera Boom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	//--Input Properties
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* SkateMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;


	//--Velocity Control Properties
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AcomulatedVelocity;

	//--Animation Properties
	//Idle Animation
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* DefaultAnim;

	//SpeedUp Animation
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* SpeedUpAnim;

	//Jump Animation
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* JumpAnim;

	//End Jump Animation float Controll
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float JumpAnimEndTime;

	//End Jump Animation bool Controll
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsPlayingJump;

public:

	int32 Score;


public:
	// Sets default values for this pawn's properties
	ASkateCharachter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	void IncreaseScore();

protected:

	/** Called for Forward movement input */
	void MoveForward(float Value);

	/** Called for Around movement input */
	void MoveAround(float Value);

	void IncreaseVelocity(float Value);

	void DecreaseVelocity(float Value);

	void QuitGame();

};
