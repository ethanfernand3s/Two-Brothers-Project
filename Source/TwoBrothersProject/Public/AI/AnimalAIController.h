// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AnimalAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
UCLASS()
class TWOBROTHERSPROJECT_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()
public:
		AAnimalAIController();
protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};
