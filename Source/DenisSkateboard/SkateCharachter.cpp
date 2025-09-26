// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateCharachter.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/LocalPlayer.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
ASkateCharachter::ASkateCharachter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));


	// Usa o capsule do ACharacter
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	//-----Only uncomment this if using as sub clas of APawn and not of ACharacter

	//--SkeltalMesh Setup for pawn
	/*SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMesh"));
	SKMesh->SetupAttachment(RootComponent);
	SKMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SKMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	SKMesh->SetSkeletalMesh(SkeletalMeshAsset.Object);*/

	//--CapsuleCollision Setup for pawn
	/*CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->InitCapsuleSize(42.f, 96.0f);
	CapsuleCollision->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CapsuleCollision;*/

	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	}


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to ar

	AcomulatedVelocity = GetCharacterMovement()->MaxWalkSpeed;
}

// Called when the game starts or when spawned
void ASkateCharachter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASkateCharachter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetCharacterMovement()->MaxWalkSpeed != AcomulatedVelocity)
	{
		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, AcomulatedVelocity + 1, DeltaTime, 1);
		UE_LOG(LogTemp, Warning, TEXT("MaxVelocity: %f"), GetCharacterMovement()->MaxWalkSpeed);
	}
}

// Called to bind functionality to input
void ASkateCharachter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASkateCharachter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASkateCharachter::MoveAround);

	PlayerInputComponent->BindAxis("SpeedUp", this, &ASkateCharachter::IncreaseVelocity);
	PlayerInputComponent->BindAxis("Break", this, &ASkateCharachter::DecreaseVelocity);

	// Liga o eixo "LookUp" à função que controla o Pitch (olhar p/ cima e baixo)
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Liga o eixo "LookAround" à função que controla o Yaw (olhar p/ os lados)
	PlayerInputComponent->BindAxis("LookAround", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

}

void ASkateCharachter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MOVE"));

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ASkateCharachter::MoveAround(float Value)
{
	if (Controller && Value != 0.0f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MOVE"));
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
	
}

void ASkateCharachter::IncreaseVelocity(float Value)
{	
	if (Controller && Value > 0.0f)
	{
		if (AcomulatedVelocity < 1000)
		{
			AcomulatedVelocity += Value;
		}
		UE_LOG(LogTemp, Warning, TEXT("AcomulatedVelocity: %f"), AcomulatedVelocity);
	}
}

void ASkateCharachter::DecreaseVelocity(float Value)
{
	if (Controller && Value < 0.0f)
	{
		if (AcomulatedVelocity > 100)
		{
			AcomulatedVelocity += Value;
		}		
		UE_LOG(LogTemp, Warning, TEXT("AcomulatedVelocity: %f"), AcomulatedVelocity);
	}
}

