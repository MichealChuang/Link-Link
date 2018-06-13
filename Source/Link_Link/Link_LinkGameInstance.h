// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/Texture2D.h"
#include "Link_LinkGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LINK_LINK_API ULink_LinkGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//单边有几个方块
	UPROPERTY(BlueprintReadWrite, Category = "Config")
	int32 Size;
	//单边长度
	UPROPERTY(BlueprintReadWrite, Category = "Config")
	int32 Length = 1200;

	UPROPERTY(BlueprintReadWrite, Category = "Jgp")
	TArray<UTexture2D*> Level_1;
	UPROPERTY(BlueprintReadWrite, Category = "Jgp")
	TArray<UTexture2D*> Level_2;
	UPROPERTY(BlueprintReadWrite, Category = "Jgp")
	TArray<UTexture2D*> Level_3;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Succeed();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();
};
