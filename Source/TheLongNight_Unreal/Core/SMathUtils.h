#pragma once

#include "CoreMinimal.h"

namespace SMathUtils
{
	inline float Clamp01(float Value)
	{
		return FMath::Clamp(Value, 0.0f, 1.0f);
	}

	inline float InverseLerp(float From, float To, float Value)
	{
		if (FMath::IsNearlyEqual(From, To))
		{
			return 0.0f;
		}

		return Clamp01((Value - From) / (To - From));
	}

	//EaseInOut like
	inline float SmoothStep01(float Alpha)
	{
		const float ClampedAlpha = Clamp01(Alpha);

		return ClampedAlpha * ClampedAlpha * (3.0f - 2.0f * ClampedAlpha);
	}

	inline float SmoothRange(float From, float To, float Value)
	{
		return SmoothStep01(InverseLerp(From, To, Value));
	}

	inline float EaseInOut01(float Alpha, float Exponent = 2.0f)
	{
		return FMath::InterpEaseInOut(
			0.0f,
			1.0f,
			Clamp01(Alpha),
			Exponent
		);
	}

	inline float EaseInOutRange(float From, float To, float Value, float Exponent = 2.0f)
	{
		return EaseInOut01(
			InverseLerp(From, To, Value),
			Exponent
		);
	}
}
