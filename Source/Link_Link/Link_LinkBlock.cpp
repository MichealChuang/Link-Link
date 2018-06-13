// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Link_LinkBlock.h"
#include "Link_LinkBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"

ALink_LinkBlock::ALink_LinkBlock()
{
	// Structure to hold one-time initialization
	ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(PlaneMesh.Object);
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ALink_LinkBlock::BlockClicked);
}

void ALink_LinkBlock::BeginPlay()
{
	Super::BeginPlay();
	DynMaterial = BlockMesh->CreateDynamicMaterialInstance(0);
}

void ALink_LinkBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	OwningGrid->ReceiveBlock(this);
}

void ALink_LinkBlock::SetMaterialTexture(UTexture2D* Jpg)
{
	DynMaterial->SetTextureParameterValue("Jpg", Jpg);
}


