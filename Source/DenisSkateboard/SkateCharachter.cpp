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

	// Usa o capsule do ACharacter
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	/*CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->InitCapsuleSize(42.f, 96.0f);
	CapsuleCollision->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CapsuleCollision;*/

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));

	/*SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMesh"));
	SKMesh->SetupAttachment(RootComponent);
	SKMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SKMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));	
	SKMesh->SetSkeletalMesh(SkeletalMeshAsset.Object);*/

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
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
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
}

// Called to bind functionality to input
void ASkateCharachter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASkateCharachter::Move);
	// Liga o eixo "LookUp" à função que controla o Pitch (olhar p/ cima e baixo)
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Liga o eixo "LookAround" à função que controla o Yaw (olhar p/ os lados)
	PlayerInputComponent->BindAxis("LookAround", this, &APawn::AddControllerYawInput);

}

void ASkateCharachter::Move(float Value)
{
	if (Controller && Value != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE"));

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

