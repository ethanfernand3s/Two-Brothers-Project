// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoBrothersProject/Public/MiniGames/Possess/PossessMiniGame.h"

#include "Components/SceneCaptureComponent2D.h"


// Sets default values
APossessMiniGame::APossessMiniGame()
{

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCapture");
}

// Called when the game starts or when spawned
void APossessMiniGame::BeginPlay()
{
	Super::BeginPlay();
	
}

