// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraZoomComponent.h"

#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UCameraZoomComponent::UCameraZoomComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCameraZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// FUNCTION CALLED BY PLAYER ACTOR
void UCameraZoomComponent::ZoomCamera(bool ZoomIn)
{
	if (ZoomIn && m_cameraComponent)
	{
		m_stopZoomingIn = false;
		CameraZoomIn();
		m_stopZoomingOut = true;
	}
	else if (!ZoomIn && m_cameraComponent)
	{
		m_stopZoomingOut = false;
		CameraZoomOut();
		m_stopZoomingIn = true;
	}
}

// ZOOMS CAMERA IN WITH TIMERS
void UCameraZoomComponent::CameraZoomIn()
{
	FTimerHandle ZoomInTimer;

	if (m_currentZoomFOV > M_CameraZoomedFOV && m_cameraComponent && !m_stopZoomingIn)
	{
		m_currentZoomFOV -= M_ZoomRate;
		m_cameraComponent->SetFieldOfView(m_currentZoomFOV);
		UE_LOG(LogTemp, Warning, TEXT("Zoom In: %f"), m_currentZoomFOV);
		GetWorld()->GetTimerManager().SetTimer(
				ZoomInTimer, this, &UCameraZoomComponent::CameraZoomIn, m_timerPlayRate);
	}
	else { ZoomInTimer.Invalidate(); }
}

void UCameraZoomComponent::CameraZoomOut()
{
	FTimerHandle ZoomOutTimer;
	if (m_currentZoomFOV < m_defaultCameraFOV && m_cameraComponent && !m_stopZoomingOut)
	{
		m_currentZoomFOV += M_ZoomRate;
		m_cameraComponent->SetFieldOfView(m_currentZoomFOV);
		UE_LOG(LogTemp, Warning, TEXT("Zoom Out: %f"), m_currentZoomFOV);
		GetWorld()->GetTimerManager().SetTimer(
				ZoomOutTimer, this, &UCameraZoomComponent::CameraZoomOut, m_timerPlayRate);
	}
	else { ZoomOutTimer.Invalidate(); }
}

void UCameraZoomComponent::SetCameraComponent(UCameraComponent* InputCamera)
{
	m_cameraComponent = InputCamera;
	m_defaultCameraFOV = InputCamera->FieldOfView;
	m_currentZoomFOV = m_defaultCameraFOV;
}

