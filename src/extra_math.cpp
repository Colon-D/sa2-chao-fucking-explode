#include "extra_math.h"

NJS_VECTOR operator+(const NJS_VECTOR& lhs, const NJS_VECTOR& rhs) {
	return {
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z
	};
}

NJS_VECTOR operator-(const NJS_VECTOR& lhs, const NJS_VECTOR& rhs) {
	return {
		lhs.x - rhs.x,
		lhs.y - rhs.y,
		lhs.z - rhs.z
	};
}

NJS_VECTOR operator*(const NJS_VECTOR& lhs, const float scalar) {
	return {
		lhs.x * scalar,
		lhs.y * scalar,
		lhs.z * scalar
	};
}

NJS_VECTOR operator/(const NJS_VECTOR& lhs, const float scalar) {
	return {
		lhs.x / scalar,
		lhs.y / scalar,
		lhs.z / scalar
	};
}

float inverse_lerp(const float a, const float b, const float value) {
	return (value - a) / (b - a);
}

float map(
	const float old_min,
	const float old_max,
	const float new_min,
	const float new_max,
	const float value
) {
	return std::lerp(new_min, new_max, inverse_lerp(old_min, old_max, value));
}
