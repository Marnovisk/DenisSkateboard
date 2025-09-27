// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateHUD.h"
#include "Engine/Canvas.h"
#include "SkateCharachter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


void ASkateHUD::DrawHUD()
{
    Super::DrawHUD();

    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    ASkateCharachter* SkateChar = Cast<ASkateCharachter>(PC->GetPawn());
    if (!SkateChar) return;

    // Pegando valores
    float CurrentSpeed = SkateChar->GetVelocity().Size();
    int32 Score = SkateChar->Score;       // você cria essa variável no character

    // HUD na tela (posição X, Y)
    FString SpeedText = FString::Printf(TEXT("Speed: %.1f"), CurrentSpeed);
    FString ScoreText = FString::Printf(TEXT("Score: %d"), Score);

    FString SpeedUpText = FString::Printf(TEXT(" 'Shift' to speed up"));
    FString SlowDownText = FString::Printf(TEXT(" 'S' to slow down"));

    FString ExitText = FString::Printf(TEXT(" 'ESC' to EXIT"));

    DrawText(SpeedText, FColor::White, 50.f, 50.f, nullptr, 1.2f, false);
    DrawText(ScoreText, FColor::Yellow, 50.f, 110.f, nullptr, 1.2f, false);

    DrawText(SpeedUpText, FColor::Green, 200.f, 50.f, nullptr, 1.2f, false);
    DrawText(SlowDownText, FColor::Green, 400.f, 50.f, nullptr, 1.2f, false);

    DrawText(ExitText, FColor::Red, 700.f, 50.f, nullptr, 1.2f, false);
}
