// Copyright Epic Games, Inc. All Rights Reserved.

#include "DenisSkateboardGameMode.h"
#include "DenisSkateboardCharacter.h"
#include "SkateCharachter.h"
#include "SkateHUD.h"
#include "GameFramework/InputSettings.h"
#include "UObject/ConstructorHelpers.h"

ADenisSkateboardGameMode::ADenisSkateboardGameMode()
{
    DefaultPawnClass = ASkateCharachter::StaticClass();
    HUDClass = ASkateHUD::StaticClass();
}

void ADenisSkateboardGameMode::StartPlay()
{
    Super::StartPlay();

    UInputSettings* InputSettings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

    if (InputSettings)
    {
        //Movement
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));

        //Jump  and speed controll
        InputSettings->AddActionMapping(FInputActionKeyMapping("Jump", EKeys::SpaceBar));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("SpeedUp", EKeys::LeftShift, 1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("Break", EKeys::S, -1.f));

        //Camera 
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::MouseY, -1.f));
        InputSettings->AddAxisMapping(FInputAxisKeyMapping("LookAround", EKeys::MouseX, 1.f));

        //Exit
        InputSettings->AddActionMapping(FInputActionKeyMapping("Exit", EKeys::Escape));

        InputSettings->SaveKeyMappings();
    }
}
