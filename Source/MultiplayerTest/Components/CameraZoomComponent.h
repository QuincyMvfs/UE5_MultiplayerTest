// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraZoomComponent.generated.h"


class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERTEST_API UCameraZoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraZoomComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void ZoomCamera(bool ZoomIn);

	UFUNCTION(BlueprintCallable)
	void CameraZoomIn();

	UFUNCTION(BlueprintCallable)
	void CameraZoomOut();

	UFUNCTION(BlueprintCallable)
	void SetCameraComponent(UCameraComponent* InputCamera);
	
public:
	UPROPERTY(EditAnywhere)
	float M_CameraZoomedFOV = 40.0f;

	UPROPERTY(EditAnywhere)
	float M_ZoomRate = 1.7f;


private:
	bool m_stopZoomingIn;
	bool m_stopZoomingOut;
	float m_defaultCameraFOV;
	float m_currentZoomFOV;
	float m_timerPlayRate = 0.005f;
	UCameraComponent* m_cameraComponent;
};
