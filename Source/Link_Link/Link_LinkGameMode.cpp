// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Link_LinkGameMode.h"
#include "Link_LinkPlayerController.h"
#include "Link_LinkPawn.h"

ALink_LinkGameMode::ALink_LinkGameMode()
{
	// no pawn by default
	DefaultPawnClass = ALink_LinkPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ALink_LinkPlayerController::StaticClass();
}
