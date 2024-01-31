// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraZoomComponent.h"

#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UCameraZoomComponent::UCameraZoomComponent()
{
	
}

void UCameraZoomComponent::SetCameraComponent(UCameraComponent* InputCamera, USpringArmComponent* SpringArm)
{
	// References
	m_cameraComponent = InputCamera;
	m_springArm = SpringArm;

	// Zooming
	m_defaultCameraFOV = InputCamera->FieldOfView;
	m_currentZoomFOV = m_defaultCameraFOV;

	// Crouching
	m_defaultHeight = m_springArm->SocketOffset.Y;
	m_currentHeight = m_defaultHeight;
	m_bottomHeight = m_defaultHeight - M_CrouchHeight;
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
		GetWorld()->GetTimerManager().SetTimer(
				ZoomOutTimer, this, &UCameraZoomComponent::CameraZoomOut, m_timerPlayRate);
	}
	else { ZoomOutTimer.Invalidate(); }
}

void UCameraZoomComponent::CrouchWithCamera(bool DropDown)
{
	if (DropDown && m_springArm)
	{
		m_stopDropping = false;
		CameraDropDown();
		m_stopRaising = true;
	}
	else if (!DropDown && m_springArm)
	{
		m_stopRaising = false;
		CameraRaiseUp();
		m_stopDropping = true;
	}
}

// LOWERS CAMERA WHEN CROUCHING
void UCameraZoomComponent::CameraDropDown()
{
	FTimerHandle CameraDropDownTimer;
	if (m_currentHeight > m_bottomHeight && m_cameraComponent && !m_stopDropping)
	{
		FVector newHeight = m_springArm->SocketOffset;
		m_currentHeight -= M_ZoomRate;
		newHeight.Z = m_currentHeight;
		m_springArm->SocketOffset = newHeight;
		GetWorld()->GetTimerManager().SetTimer(
				CameraDropDownTimer, this, &UCameraZoomComponent::CameraDropDown, m_timerPlayRate);
	}
	else { CameraDropDownTimer.Invalidate(); }
}

// RAISES CAMERA WHEN UN-CROUCHING
void UCameraZoomComponent::CameraRaiseUp()
{
	FTimerHandle CameraDropDownTimer;
	if (m_currentHeight < m_defaultHeight && m_cameraComponent && !m_stopRaising)
	{
		FVector newHeight = m_springArm->SocketOffset;
		m_currentHeight += M_ZoomRate;
		newHeight.Z = m_currentHeight;
		m_springArm->SocketOffset = newHeight;
		GetWorld()->GetTimerManager().SetTimer(
				CameraDropDownTimer, this, &UCameraZoomComponent::CameraRaiseUp, m_timerPlayRate);
	}
	else { CameraDropDownTimer.Invalidate(); }
}

