#include "stdafx.h"

#include "roengine/camera.h"
#include <gl/glu.h>
#include <math.h>

Camera::Camera() {
	eye.set(20, 20, 20);
//	speed = 1;
	fx = vx = fy = vy = 0;
	zoom = zoom_a = 0;
}

Camera::~Camera() {
}

void Camera::Update() {
	//return;
	/*if (v < f) {
		v += speed;
		if (v > f)
			v = f;
		eye.rotateY(speed, dest);
	}*/
	if (vx > 0.f) {
		vx -= fx;
		if (vx <= 0.f)
			vx = 0.f;
		else
			eye.rotateX(vx / 20.0f, dest);
	}
	else if (vx < 0.f) {
		vx -= fx;
		if (vx >= 0.f)
			vx = 0.f;
		else
			eye.rotateX(vx / 20.0f, dest);
	}
	if (vy > 0.f) {
		vy -= fy;
		if (vy <= 0.f)
			vy = 0.f;
		else
			eye.rotateY(vy / 20.0f, dest);
	}
	else if (vy < 0.f) {
		vy -= fy;
		if (vy >= 0.f)
			vy = 0.f;
		else
			eye.rotateY(vy / 20.0f, dest);
	}

	Vector3f dir = dest - eye;
	if (zoom > 0.f) {
		if (dir.size() <= 60.f) {
			zoom = 0;
			return;
		}
		zoom -= zoom / 5.f;
		if (zoom <= 0.f)
			zoom = 0.f;
		else {
			dir /= dir.size();
			dir *= zoom;

			eye += dir;
		}
	}
	else if (zoom < 0.f) {
		if (dir.size() >= 1000.f) {
			zoom = 0;
			return;
		}
		zoom -= zoom / 5.f;
		if (zoom >= 0.f)
			zoom = 0.f;
		else {
			dir /= dir.size();
			dir *= zoom;

			eye += dir;
		}
	}
}

void Camera::RotateY(float f) {
	//this->f =  v + f;
	vy += f;
	this->fy = vy / 10.0f;
	//f *= 10;
	//f = f * 3.141596 / 180.0f;
	//eye.rotateY(f, dest);
	//dest.rotateY(f, dest);
}

void Camera::RotateX(float f) {
	vx += f;
	this->fx = vx / 10.0f;

//	eye.rotateX(f, dest);
	//eye.rotateX(f);
//	eye.rotateZ(f, dest);
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
	if (dir.size() <= 60)
		return;
	zoom += f;
	zoom_a = zoom / 10.f;
	return;
	dir /= dir.size();
	dir *= f;

	//eye += dir;
//	zoom += dir;
}

void Camera::ZoomOut(float f) {
	Vector3f dir = dest - eye;
	if (dir.size() >= 1000)
		return;
	zoom -= f;
	zoom_a = zoom / 10.f;
	return;
	dir /= dir.size();
	dir *= f;

	//eye -= dir;
//	zoom -= dir;
}


Vector3f& Camera::getEye() { return(eye); }
Vector3f& Camera::getDest() { return(dest); }
const Vector3f& Camera::getEye() const { return(eye); }
const Vector3f& Camera::getDest() const  { return(dest); }

ro::CDir Camera::getDirection() const {
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
		return(ro::DIR_N);
	else if (angle <= 67.5)
		return(ro::DIR_NE);
	else if (angle <= 112.5)
		return(ro::DIR_E);
	else if (angle <= 157.5)
		return(ro::DIR_SE);
	else if (angle <= 202.5)
		return(ro::DIR_S);
	else if (angle <= 247.5)
		return(ro::DIR_SW);
	else if (angle <= 292.5)
		return(ro::DIR_W);
	else if (angle <= 337.5)
		return(ro::DIR_NW);

	return(ro::DIR_N);
}


