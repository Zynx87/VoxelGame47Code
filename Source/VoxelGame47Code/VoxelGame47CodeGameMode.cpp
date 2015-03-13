// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "VoxelGame47Code.h"
#include "VoxelGame47CodeGameMode.h"
#include "VoxelGame47CodeHUD.h"
#include "VoxelGame47CodeCharacter.h"

AVoxelGame47CodeGameMode::AVoxelGame47CodeGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AVoxelGame47CodeHUD::StaticClass();
}
