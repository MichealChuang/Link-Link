// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Link_LinkPawn.h"
#include "Link_LinkBlock.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ALink_LinkPawn::ALink_LinkPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

