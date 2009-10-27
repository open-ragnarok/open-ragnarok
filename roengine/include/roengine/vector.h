/* $Id$ */
#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>

template<typename T>
class Vector3 {
protected:
	T _data[3];
public:
	static const Vector3<T> UNIT_X;
	static const Vector3<T> UNIT_Y;
	static const Vector3<T> UNIT_Z;

	/** Basic vector constructor, with zero length */
	Vector3() {
		_data[0] = _data[1] = _data[2] = 0;
	}


	Vector3(const T& v) {
		_data[0] = _data[1] = _data[2] = v;
	}

	Vector3(const T& v1, const T& v2, const T& v3) {
		_data[0] = v1;
		_data[1] = v2;
		_data[2] = v3;
	}

	Vector3(const T* v) {
		_data[0] = v[0];
		_data[1] = v[1];
		_data[2] = v[2];
	}

	/** Basic copy constructor. Initializes vector from another vector */
	Vector3(const Vector3<T>& v) {
		_data[0] = v._data[0];
		_data[1] = v._data[1];
		_data[2] = v._data[2];
	}

	/** Rotates the current vector around the X Axis */
	Vector3<T>& rotateX(const T& angle) {
		T s = sin(angle);
		T c = cos(angle);
		
		T ny = c * _data[1] - s * _data[2];
		T nz = s * _data[1] + c * _data[2];

		_data[1] = ny;
		_data[2] = nz;

		return(*this);
	}

	Vector3<T>& rotateX(const T& angle, const Vector3<T>& v) {
		T s = sin(angle);
		T c = cos(angle);

		sub(v);
		
		T ny = c * _data[1] - s * _data[2];
		T nz = s * _data[1] + c * _data[2];

		_data[1] = ny;
		_data[2] = nz;

		add(v);

		return(*this);
	}

	/** Rotates the current vector around the Y Axis */
	Vector3<T>& rotateY(const T& angle) {
		T s = sin(angle);
		T c = cos(angle);
		
		T nx =  c * _data[0] + s * _data[2];
		T nz = -s * _data[0] + c * _data[2];

		_data[0] = nx;
		_data[2] = nz;

		return(*this);
	}

	Vector3<T>& rotateY(const T& angle, const Vector3<T>& v) {
		T s = sin(-angle);
		T c = cos(-angle);

		sub(v);
		
		T nx =  c * _data[0] + s * _data[2];
		T nz = -s * _data[0] + c * _data[2];

		_data[0] = nx;
		_data[2] = nz;

		add(v);

		return(*this);
	}

	/** Rotates the current vector around the Z Axis */
	Vector3<T>& rotateZ(const T& angle) {
		T s = sin(angle);
		T c = cos(angle);
		
		T nx = c * _data[0] - s * _data[1];
		T ny = s * _data[0] + c * _data[1];

		_data[0] = nx;
		_data[1] = ny;

		return(*this);
	}

	Vector3<T>& rotateZ(const T& angle, const Vector3<T>& v) {
		T s = sin(angle);
		T c = cos(angle);

		sub(v);
		
		T nx = c * _data[0] - s * _data[1];
		T ny = s * _data[0] + c * _data[1];

		_data[0] = nx;
		_data[1] = ny;

		add(v);

		return(*this);
	}


	T& operator[] (unsigned int i) {
		return(_data[i]);
	}

	const T& operator[] (unsigned int i) const {
		return(_data[i]);
	}

	void set(const T& v1, const T& v2, const T& v3) {
		_data[0] = v1;
		_data[1] = v2;
		_data[2] = v3;
	}

	void set(const T* v) {
		_data[0] = v[0];
		_data[1] = v[1];
		_data[2] = v[2];
	}

	void setX(const T& v) {
		_data[0] = v;
	}

	void setY(const T& v) {
		_data[1] = v;
	}

	void setZ(const T& v) {
		_data[2] = v;
	}

	T& getX() {
		return(_data[0]);
	}

	const T& getX() const {
		return(_data[0]);
	}

	T& getY() {
		return(_data[1]);
	}

	const T& getY() const {
		return(_data[1]);
	}

	T& getZ() {
		return(_data[2]);
	}

	const T& getZ() const {
		return(_data[2]);
	}

	/**
	 * Calculates the dot product between this vector and another given vector.
	 */
	T dot(const Vector3<T>& v) const {
		T ret = 0;
		for (int i = 0; i < 3; i++)
			ret += _data[i] * v._data[i];

		return(ret);
	}

	/** Returns the angle between two vectors (in radians)
	 */
	T angle(const Vector3<T>& v) const {
		if (v.size() == 0)
			return(0);
		if (size() == 0)
			return(0);

		Vector3<T> v1, v2;
		v1 = *this;
		v1 /= v1.size();
		v2 = v;
		v2 /= v2.size();
		T ret = v1.dot(v2);

		ret = acos(ret);

		return(ret);
	}

	/** Adds one vector with another */
	Vector3<T>& add(const Vector3<T>& v) {
		_data[0] += v._data[0];
		_data[1] += v._data[1];
		_data[2] += v._data[2];
		return(*this);
	}

	/** Subtracts one vector with another */
	Vector3<T>& sub(const Vector3<T>& v) {
		_data[0] -= v._data[0];
		_data[1] -= v._data[1];
		_data[2] -= v._data[2];
		return(*this);
	}

	Vector3<T>& operator = (const Vector3<T>& v) {
		_data[0] = v._data[0];
		_data[1] = v._data[1];
		_data[2] = v._data[2];
		return(*this);
	}

	Vector3<T> operator + (const Vector3<T>& v) const {
		T x[3];
		x[0] = _data[0] + v._data[0];
		x[1] = _data[1] + v._data[1];
		x[2] = _data[2] + v._data[2];
		return(Vector3<T>(x));
	}

	/**
	 * Calculates the cross product
	 */
	Vector3<T> operator * (const Vector3<T>& v) const {
		T x[3];
		// Yeah, cheating. I'm lazy. Sue me!
		x[0] = _data[1] * v._data[2] - _data[2] * v._data[1];
		x[1] = _data[2] * v._data[0] - _data[0] * v._data[2];
		x[2] = _data[0] * v._data[1] - _data[1] * v._data[0];
		return(Vector3<T>(x));
	}

	/**
	 * Calculates the cross product
	 */
	Vector3<T>& operator *= (const Vector3<T>& v) {
		T x[3];
		// Yeah, cheating. I'm lazy. Sue me!
		x[0] = _data[1] * v._data[2] - _data[2] * v._data[1];
		x[1] = _data[2] * v._data[0] - _data[0] * v._data[2];
		x[2] = _data[0] * v._data[1] - _data[1] * v._data[0];

		_data[0] = x[0];
		_data[1] = x[1];
		_data[2] = x[2];
		return(*this);
	}

	Vector3<T>& operator += (const Vector3<T>& v) {
		_data[0] += v._data[0];
		_data[1] += v._data[1];
		_data[2] += v._data[2];
		return(*this);
	}

	Vector3<T> operator - (const Vector3<T>& v) const {
		T x[3];
		x[0] = _data[0] - v._data[0];
		x[1] = _data[1] - v._data[1];
		x[2] = _data[2] - v._data[2];
		return(Vector3<T>(x));
	}

	Vector3<T>& operator -= (const Vector3<T>& v) {
		_data[0] -= v._data[0];
		_data[1] -= v._data[1];
		_data[2] -= v._data[2];
		return(*this);
	}

	Vector3<T> operator * (const T& v) const {
		T x[3];
		x[0] = _data[0] * v;
		x[1] = _data[1] * v;
		x[2] = _data[2] * v;
		return(Vector3<T>(x));
	}

	Vector3<T>& operator *= (const T& v) {
		_data[0] *= v;
		_data[1] *= v;
		_data[2] *= v;
		return(*this);
	}

	Vector3<T> operator / (const T& v) const {
		T x[3];
		x[0] = _data[0] / v;
		x[1] = _data[1] / v;
		x[2] = _data[2] / v;
		return(Vector3<T>(x));
	}

	Vector3<T>& operator /= (const T& v) {
		_data[0] /= v;
		_data[1] /= v;
		_data[2] /= v;
		return(*this);
	}

	T size() const {
		return(sqrt(_data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2]));
	}

	bool operator == (const Vector3<T>& v) const {
		return((_data[0] == v._data[0]) && (_data[1] == v._data[1]) && (_data[2] == v._data[2]));
	}

	bool operator != (const Vector3<T>& v) const {
		return((_data[0] != v._data[0]) || (_data[1] != v._data[1]) || (_data[2] != v._data[2]));
	}

	bool operator > (const Vector3<T>& v) const {
		return(size() > v.size());
	}

	bool operator < (const Vector3<T>& v) const {
		return(size() < v.size());
	}

	bool operator >= (const Vector3<T>& v) const {
		return(size() >= v.size());
	}

	bool operator <= (const Vector3<T>& v) const {
		return(size() <= v.size());
	}
};

template <typename T>
const Vector3<T> Vector3<T>::UNIT_X = Vector3<T>(1, 0, 0);
template <typename T>
const Vector3<T> Vector3<T>::UNIT_Y = Vector3<T>(0, 1, 0);
template <typename T>
const Vector3<T> Vector3<T>::UNIT_Z = Vector3<T>(0, 0, 1);

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;



#endif /* __VECTOR_H */
