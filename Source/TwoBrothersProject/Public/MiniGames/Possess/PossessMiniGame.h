// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PossessMiniGame.generated.h"

UCLASS()
class TWOBROTHERSPROJECT_API APossessMiniGame : public AActor
{
	GENERATED_BODY()

public:

	APossessMiniGame();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

};
