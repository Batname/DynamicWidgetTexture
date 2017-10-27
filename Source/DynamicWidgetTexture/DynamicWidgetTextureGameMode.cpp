// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DynamicWidgetTextureGameMode.h"
#include "DynamicWidgetTexturePlayerController.h"
#include "DynamicWidgetTextureCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADynamicWidgetTextureGameMode::ADynamicWidgetTextureGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADynamicWidgetTexturePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}