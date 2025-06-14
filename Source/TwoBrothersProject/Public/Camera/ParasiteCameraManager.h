// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ParasiteCameraManager.generated.h"

class UPostProcessComponent;
class ABaseAnimalCharacter;
/**
 * 
 */
UCLASS()
class TWOBROTHERSPROJECT_API AParasiteCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	virtual void UpdateCamera(float DeltaTime) override;
private:
	void UpdateParasiteCam(float DT);
	void UpdateHostCam(float DT, ABaseAnimalCharacter* Host);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UPostProcessComponent> ParasiteVisionPostProcess;
};
