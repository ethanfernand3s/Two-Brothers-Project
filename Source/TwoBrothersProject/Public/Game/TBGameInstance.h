// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API UTBGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "UMG", meta = (AllowPrivateAccess))
	float DragTriggerDistance = 0.f;
};
