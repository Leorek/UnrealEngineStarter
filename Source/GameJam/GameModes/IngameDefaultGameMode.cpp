// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameDefaultGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"

float AIngameDefaultGameMode::ActualTimeGame() const
{
	UWorld* _world = GetWorld();

	if (_world) {
		return _world->TimeSince(TimeGameStart);
	}
	else {
		return 0;
	}
}