// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ParasiteCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseAnimalCharacter.h"

void AParasiteCameraManager::UpdateCamera(float DT)
{
	if (APlayerController* PC = PCOwner)
	{
		if (ABaseAnimalCharacter* Host = Cast<ABaseAnimalCharacter>(PC->GetPawn()))
			UpdateHostCam(DT, Host);
		else
			UpdateParasiteCam(DT);      
	}
	Super::UpdateCamera(DT);
}

void AParasiteCameraManager::UpdateParasiteCam(float DT)
{
	if (!PCOwner) return;
	if (APawn* Pawn = PCOwner->GetPawn())
	{
		if (UCameraComponent* Cam = Pawn->FindComponentByClass<UCameraComponent>())
		{
			Cam->GetCameraView(DT, ViewTarget.POV);   // fills the internal POV struct
		}
	}
}

void AParasiteCameraManager::UpdateHostCam(float DT, ABaseAnimalCharacter* Host)
{
	if (!Host) return;
	if (UCameraComponent* Cam = Host->FindComponentByClass<UCameraComponent>())
	{
		Cam->GetCameraView(DT, ViewTarget.POV);
	}
}
