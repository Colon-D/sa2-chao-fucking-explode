#pragma once
#include <SA2ModLoader.h>
#include <algorithm>
#include <cmath>

NJS_VECTOR operator+(const NJS_VECTOR& lhs, const NJS_VECTOR& rhs);

NJS_VECTOR operator-(const NJS_VECTOR& lhs, const NJS_VECTOR& rhs);

NJS_VECTOR operator*(const NJS_VECTOR& lhs, float scalar);

NJS_VECTOR operator/(const NJS_VECTOR& lhs, float scalar);

float inverse_lerp(float a, float b, float value);

float map(
	float old_min,
	float old_max,
	float new_min,
	float new_max,
	float value
);

template<typename value_t>
void apply_ratio(
	value_t& value,
	const int clamp_min,
	const int clamp_max,
	const int delta_percent_min,
	const int delta_percent_max,
	const float ratio
) {
	const int clamp_range{ clamp_max - clamp_min };
	value = static_cast<value_t>(std::clamp(
		(
			value
			+ static_cast<int>(
				std::lerp(
					delta_percent_min * clamp_range,
					delta_percent_max * clamp_range,
					ratio
				)
			)
		),
		clamp_min,
		clamp_max
	));
}
