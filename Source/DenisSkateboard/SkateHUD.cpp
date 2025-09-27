// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateHUD.h"
#include "Engine/Canvas.h"
#include "SkateCharachter.h"
#include "GameFramework/PlayerController.h"

void ASkateHUD::DrawHUD()
{
    Super::DrawHUD();

    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    ASkateCharachter* SkateChar = Cast<ASkateCharachter>(PC->GetPawn());
    if (!SkateChar) return;

    // Pegando valores
    float CurrentSpeed = SkateChar->GetVelocity().Size();
    float JumpForce = 800; // exemplo, se usa CharacterMovement
    int32 Score = SkateChar->Score;       // você cria essa variável no character

    // HUD na tela (posição X, Y)
    FString SpeedText = FString::Printf(TEXT("Speed: %.1f"), CurrentSpeed);
    FString JumpText = FString::Printf(TEXT("Jump Force: %.1f"), JumpForce);
    FString ScoreText = FString::Printf(TEXT("Score: %d"), Score);

    DrawText(SpeedText, FColor::White, 50.f, 50.f, nullptr, 1.2f, false);
    DrawText(JumpText, FColor::Green, 50.f, 80.f, nullptr, 1.2f, false);
    DrawText(ScoreText, FColor::Yellow, 50.f, 110.f, nullptr, 1.2f, false);
}
