// Copyright Epic Games, Inc. All Rights Reserved.

#include "DenisSkateboardGameMode.h"
#include "DenisSkateboardCharacter.h"
#include "SkateCharachter.h"
#include "GameFramework/InputSettings.h"
#include "UObject/ConstructorHelpers.h"

ADenisSkateboardGameMode::ADenisSkateboardGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

    DefaultPawnClass = ASkateCharachter::StaticClass();
}

void ADenisSkateboardGameMode::StartPlay()
{
	Super::StartPlay();

	UInputSettings* InputSettings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

    if (InputSettings)
    {
        // Movimentação
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("Break", EKeys::S, -1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));

        // Pulo e boost
        InputSettings->AddActionMapping(FInputActionKeyMapping("Jump", EKeys::SpaceBar));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("SpeedUp", EKeys::LeftShift, 1.f));

        InputSettings->AddAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::MouseY, -1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("LookAround", EKeys::MouseX, 1.f));

        InputSettings->SaveKeyMappings();

        UE_LOG(LogTemp, Display, TEXT("Input mappings configurados via GameMode (C++)."));
    }
}
