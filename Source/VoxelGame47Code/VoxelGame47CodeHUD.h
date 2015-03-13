// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "VoxelGame47CodeHUD.generated.h"

UCLASS()
class AVoxelGame47CodeHUD : public AHUD
{
	GENERATED_BODY()

public:
	AVoxelGame47CodeHUD(const FObjectInitializer& ObjectInitializer);

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

