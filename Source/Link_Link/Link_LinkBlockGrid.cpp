// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Link_LinkBlockGrid.h"
#include "Link_LinkBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Link_LinkGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ALink_LinkBlockGrid::ALink_LinkBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	//Initialize time
	time = 120;
	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::FromString(FString("Time : ").Append(FString::FromInt(time))));
	ScoreText->SetupAttachment(DummyRoot);
}

void ALink_LinkBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Set defaults size
	GI = Cast<ULink_LinkGameInstance>(GetGameInstance());
	if (GI == nullptr)	return;
	Size = GI->Size;
	BlockSpacing = GI->Length / GI->Size;

	// Number of blocks
	NumBlocks = Size * Size;

	//initialize TextureArray
	TArray<UTexture2D*> TextureArray;
	if (GI->Level_1.Num() > 0)
	{
		switch (Size)
		{
			case 6:
				TextureArray = GI->Level_1; break;
			case 8:
				TextureArray = GI->Level_2; break;
			case 10:
				TextureArray = GI->Level_3; break;
			default:
				return;
		}
	}
	TArray<UTexture2D*>ArrayTemp = TextureArray;
	TextureArray.Append(ArrayTemp);

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ALink_LinkBlock* NewBlock = GetWorld()->SpawnActor<ALink_LinkBlock>(BlockLocation, FRotator(0,90,0));
		NewBlock->SetActorScale3D(GetActorScale3D()* BlockSpacing /256);
		int32 Index = FMath::RandRange(0, NumBlocks - BlockIndex - 1);
		NewBlock->SetMaterialTexture(TextureArray[Index]);
		NewBlock->MaterialTexture = TextureArray[Index];
		TextureArray.RemoveAt(Index);
		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
		}
	}
	GetWorldTimerManager().SetTimer(GameOverHandle, this, &ALink_LinkBlockGrid::GameOver, 1, true);
}

void ALink_LinkBlockGrid::ReceiveBlock(ALink_LinkBlock* TempBlock)
{
	if (!FirstBlock)
	{
		FirstBlock = TempBlock;
		FirstBlock->DynMaterial->SetScalarParameterValue("Num", 1);
	}
	else if (FirstBlock == TempBlock)
	{
		FirstBlock->DynMaterial->SetScalarParameterValue("Num", 0);
		FirstBlock = nullptr;
	}
	else
	{
		if (FirstBlock->MaterialTexture == TempBlock->MaterialTexture)
		{
			FirstBlock->Destroy();
			FirstBlock = nullptr;
			TempBlock->Destroy();
			NumBlocks -= 2;
			if (NumBlocks == 0)
			{
				GetWorldTimerManager().ClearTimer(GameOverHandle);
				switch (Size)
				{
				case 6:
					UGameplayStatics::OpenLevel(this, "Level_2"); 
					GI->Size = 8; break;
				case 8:
					UGameplayStatics::OpenLevel(this, "Level_3"); 
					GI->Size = 10; break;
				case 10:
					GI->Succeed();
				}
			}
		}
		else
		{
			FirstBlock->DynMaterial->SetScalarParameterValue("Num", 0);
			FirstBlock = nullptr;
		}
	}
}


void ALink_LinkBlockGrid::GameOver()
{
	time -= 1;
	ScoreText->SetText(FText::FromString(FString("Time : ").Append(FString::FromInt(time))));
	if (time == 0)
	{
		GetWorldTimerManager().ClearTimer(GameOverHandle);
		GI->GameOver();
	}
	
}

#undef LOCTEXT_NAMESPACE
