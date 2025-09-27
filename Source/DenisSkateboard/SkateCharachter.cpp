// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateCharachter.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/LocalPlayer.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
ASkateCharachter::ASkateCharachter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Assets references finder
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Animations/Y_Bot_Skateboarding.Y_Bot_Skateboarding"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DefaultAnimAsset(TEXT("/Game/Animations/Y_Bot_Skateboarding_Anim.Y_Bot_Skateboarding_Anim"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> SpeedUpAnimAsset(TEXT("/Game/Animations/Y_Bot_Skateboarding_Row.Y_Bot_Skateboarding_Row"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> JumpAnimAsset(TEXT("/Game/Animations/Y_Bot_Jumping.Y_Bot_Jumping"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Game/Skate/skateboard.skateboard"));

	// Usa o capsule do ACharacter
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Assets references setter
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	}

	if (DefaultAnimAsset.Succeeded())
	{
		DefaultAnim = DefaultAnimAsset.Object;
		GetMesh()->PlayAnimation(DefaultAnim, true);
	}

	if (SpeedUpAnimAsset.Succeeded())
	{
		SpeedUpAnim = SpeedUpAnimAsset.Object;
	}

	if (JumpAnimAsset.Succeeded())
	{
		JumpAnim = JumpAnimAsset.Object;
	}


	//Skateboart StaticMesh setup
	SMSkateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMSkateMesh"));
	SMSkateMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
	SMSkateMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SMSkateMesh->SetupAttachment(GetMesh());

	if (StaticMeshAsset.Succeeded())
	{
		SMSkateMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	//Camera boom setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//Follow camera setup
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to ar

	//Velocity controll setup
	GetCharacterMovement()->BrakingDecelerationWalking = 200.f;
	AcomulatedVelocity = GetCharacterMovement()->MaxWalkSpeed;

	//Variables Setup
	Score = 0;
	JumpAnimEndTime = 0.f;
	bIsPlayingJump = false;
}

// Called when the game starts or when spawned
void ASkateCharachter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->PlayAnimation(DefaultAnim, true);
}

// Called every frame
void ASkateCharachter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlayingJump && GetWorld()->GetTimeSeconds() >= JumpAnimEndTime)
	{
		bIsPlayingJump = false;

		if (AcomulatedVelocity > 600 && SpeedUpAnim)
		{
			GetMesh()->PlayAnimation(SpeedUpAnim, true);
		}
		else if (DefaultAnim)
		{
			GetMesh()->PlayAnimation(DefaultAnim, true);
		}
	}

	if (GetCharacterMovement()->MaxWalkSpeed != AcomulatedVelocity)
	{
		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, AcomulatedVelocity, DeltaTime, 1);
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


	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookAround", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &ASkateCharachter::QuitGame);

}

void ASkateCharachter::IncreaseScore()
{
	Score += 1;
}

void ASkateCharachter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}

void ASkateCharachter::MoveAround(float Value)
{
	if (Controller && Value != 0.0f)
	{
		AddActorLocalRotation(FRotator(0.f, Value, 0.f));
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
	}
}

void ASkateCharachter::QuitGame()
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		UE_LOG(LogTemp, Warning, TEXT("ESC pressed - quitting game..."));
		UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
	}
}

void ASkateCharachter::Jump()
{
	Super::Jump();

	if (JumpAnim && GetMesh()->GetAnimationMode() == EAnimationMode::AnimationSingleNode)
	{
		GetMesh()->PlayAnimation(JumpAnim, false);

		float Duration = JumpAnim->GetMaxCurrentTime(); // duração da animação
		JumpAnimEndTime = GetWorld()->GetTimeSeconds() + Duration;
		bIsPlayingJump = true;
	}
}
