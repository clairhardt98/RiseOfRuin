// Copyright Epic Games, Inc. All Rights Reserved.

#include "RiseOfRuinGameMode.h"
#include "RiseOfRuinCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARiseOfRuinGameMode::ARiseOfRuinGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Player/BP_Player.BP_Player'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
