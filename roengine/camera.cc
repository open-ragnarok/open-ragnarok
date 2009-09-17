#include "stdafx.h"

#include "roengine/camera.h"
#include <gl/glu.h>

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

