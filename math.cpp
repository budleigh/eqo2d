#include "math.h"

Vector Vector::operator+(Vector other) const {
	Vector result;

	result.x = x + other.x;
	result.y = y + other.y;

	return result;
}

Vector Vector::operator*(double scalar) const {
	Vector result;

	result.x = x * scalar;
	result.y = y * scalar;

	return result;
}

Vector Vector::applyTransform(TransformMatrix transform) const {
	Vector result;

	result.x = (transform.x0 * x) + (transform.x1 * y);
	result.y = (transform.y0 * x) + (transform.y1 * y);

	return result;
}

TransformMatrix TransformMatrix::operator*(double scalar) const {
	TransformMatrix result;

	result.x0 = x0 * scalar;
	result.y0 = y0 * scalar;
	result.x1 = x1 * scalar;
	result.y1 = y1 * scalar;

	return result;
}
