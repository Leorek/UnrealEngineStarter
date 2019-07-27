// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class GAMEJAM_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Dimensions")
	int32 Width;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile Dimensions")
	int32 Height;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
