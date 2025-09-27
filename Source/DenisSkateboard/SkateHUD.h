// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SkateHUD.generated.h"

/**
 * 
 */
UCLASS()
class DENISSKATEBOARD_API ASkateHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void DrawHUD() override;
};
