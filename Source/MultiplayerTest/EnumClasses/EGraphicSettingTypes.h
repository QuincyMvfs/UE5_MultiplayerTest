#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGraphicSettingTypes:uint8
{
	Overall,
	ResolutionScale,
	ViewDistance,
	AntiAliasing,
	PostProcessing,
	Shadows,
	GlobalIllumination,
	Reflections,
	Textures,
	Effects,
	Foliage,
	Shading,
};
