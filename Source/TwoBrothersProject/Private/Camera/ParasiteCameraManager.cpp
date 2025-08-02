// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ParasiteCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseAnimalCharacter.h"

/* DEPRECATED - HANDLED BY TRYPOSSESS ABILITY NOW
void AParasiteCameraManager::UpdateCamera(float DT)
{
	Super::UpdateCamera(DT);
	
	if (APlayerController* PC = PCOwner)
	{
		// TODO: Need to move to on event such as Controller changed or etc. cuz this ain't performant
		if (ABaseAnimalCharacter* Host = Cast<ABaseAnimalCharacter>(PC->GetPawn()))
			UpdateHostCam(DT, Host);
		else
			UpdateParasiteCam(DT);      
	}
}

void AParasiteCameraManager::UpdateParasiteCam(float DT)
{
	if (!PCOwner) return;
	if (APawn* Pawn = PCOwner->GetPawn())
	{
		// TODO: Finding by class every tick is fine but you can listen to APawn::NotifyControllerChanged() or OnPossessed to store ParasiteCam, HostCam once.
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
*/