// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraZoomComponent.generated.h"


class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERTEST_API UCameraZoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraZoomComponent();

public:	
	
	UFUNCTION(BlueprintCallable)
	void ZoomCamera(bool ZoomIn);

	UFUNCTION(BlueprintCallable)
	void CrouchWithCamera(bool DropDown);

	UFUNCTION(BlueprintCallable)
	void SetCameraComponent(UCameraComponent* InputCamera, USpringArmComponent* SpringArm);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float M_CameraZoomedFOV = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float M_ZoomRate = 1.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch Cam")
	float M_CrouchHeight = 50.0f;

private:
	void CameraZoomIn();
	void CameraZoomOut();

	void CameraDropDown();
	void CameraRaiseUp();

private:
	float m_timerPlayRate = 0.005f;
	
	// Zooming
	bool m_stopZoomingIn;
	bool m_stopZoomingOut;
	float m_defaultCameraFOV;
	float m_currentZoomFOV;
	UCameraComponent* m_cameraComponent;

	// Crouching
	bool m_stopDropping;
	bool m_stopRaising;
	float m_defaultHeight;
	float m_currentHeight;
	float m_bottomHeight;
	USpringArmComponent* m_springArm;
};
