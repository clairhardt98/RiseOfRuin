#include "DefaultGameMode.h"
#include "RiseOfRuin/RiseOfRuin.h"
#include "RiseOfRuin/Player/PlayerCharacter.h"

ADefaultGameMode::ADefaultGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PRINT_CALLINFO();
	PRINT_LOG(TEXT("DefaultGameMode : "), TEXT("Debug Test"));
}

