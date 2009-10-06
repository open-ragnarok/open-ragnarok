#include "stdafx.h"

#include "roengine/camera.h"
#include <gl/glu.h>
#include <math.h>

Camera::Camera() {
	eye.set(20, 20, 20);
}

Camera::~Camera() {
}

void Camera::Rotate(float f) {
	eye.rotateY(f, dest);
}

void Camera::TranslateDestTo(const Vector3f& v) {
	Vector3f offset = v - dest;
	eye += offset;
	dest += offset;
}

void Camera::Translate(const Vector3f& v) {
	eye += v;
	dest += v;
}

void Camera::TranslateX(const float& f) {
	eye[0] += f;
	dest[0] += f;
}

void Camera::TranslateY(const float& f) {
	eye[1] += f;
	dest[1] += f;
}

void Camera::TranslateZ(const float& f) {
	eye[2] += f;
	dest[2] += f;
}

void Camera::Look() const {
	// printf("eye: %.2f %.2f %.2f\t\r", eye[0], eye[1], eye[2]);
	gluLookAt(eye[0], eye[1], eye[2], dest[0], dest[1], dest[2], 0, 1, 0);
}

void Camera::setEye(const Vector3f& v) {
	eye = v;
}

void Camera::setDest(const Vector3f& v) {
	dest = v;
}

void Camera::ZoomIn(float f) {
	Vector3f dir = dest - eye;
	dir /= dir.size();
	dir *= f;

	eye += dir;
}

void Camera::ZoomOut(float f) {
	Vector3f dir = dest - eye;
	dir /= dir.size();
	dir *= f;

	eye -= dir;
}


Vector3f& Camera::getEye() { return(eye); }
Vector3f& Camera::getDest() { return(dest); }
const Vector3f& Camera::getEye() const { return(eye); }
const Vector3f& Camera::getDest() const  { return(dest); }

RO::CDir Camera::getDirection() const {
	Vector3f dir = dest - eye;
	dir[1] = 0;
	dir /= dir.size();

	float a_cos = dir.dot(Vector3f(0, 0, -1));
	float angle = acos(a_cos) * 180 / 3.1415f;

	if (dir[0] < 0)
		angle = 360 - angle;

	/*
	printf("Camera Angle: %.2f (%.2f)\t", angle, a_cos);
	printf("Camera direction: %.2f %.2f\t", dir[0], dir[2]);
	printf("\r");
	*/

	if (angle <= 22.5)
		return(RO::DIR_N);
	else if (angle <= 67.5)
		return(RO::DIR_NE);
	else if (angle <= 112.5)
		return(RO::DIR_E);
	else if (angle <= 157.5)
		return(RO::DIR_SE);
	else if (angle <= 202.5)
		return(RO::DIR_S);
	else if (angle <= 247.5)
		return(RO::DIR_SW);
	else if (angle <= 292.5)
		return(RO::DIR_W);
	else if (angle <= 337.5)
		return(RO::DIR_NW);

	return(RO::DIR_N);
}


