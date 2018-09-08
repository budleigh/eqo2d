#pragma once

class TransformMatrix {
public:
	TransformMatrix() {}
	TransformMatrix(double x0, double y0, double x1, double y1) : x0(x0), y0(y0), x1(x1), y1(y1) {}

	double x0;
	double y0;
	double x1;
	double y1;

	TransformMatrix operator*(double scalar) const;
};

class Vector {
public:
	Vector() {}
	Vector(double x, double y) : x(x), y(y) {}

	double x;
	double y;

	Vector operator+(Vector other) const;
	Vector operator*(double scalar) const;

	Vector applyTransform(TransformMatrix transform) const;
};
