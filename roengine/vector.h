#ifndef __VECTOR_H
#define __VECTOR_H

#include <math.h>

template<typename T>
class Vector3 {
protected:
	T _data[3];
public:
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

	Vector3(const Vector3<T>& v) {
		_data[0] = v._data[0];
		_data[1] = v._data[1];
		_data[2] = v._data[2];
	}

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
		T s = sin(angle);
		T c = cos(angle);

		sub(v);
		
		T nx =  c * _data[0] + s * _data[2];
		T nz = -s * _data[0] + c * _data[2];

		_data[0] = nx;
		_data[2] = nz;

		add(v);

		return(*this);
	}

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

	Vector3<T>& add(const Vector3<T>& v) {
		_data[0] += v._data[0];
		_data[1] += v._data[1];
		_data[2] += v._data[2];
		return(*this);
	}

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
		float x[3];
		x[0] = _data[0] + v._data[0];
		x[0] = _data[1] + v._data[1];
		x[0] = _data[2] + v._data[2];
		return(Vector3<T>(x));
	}

	Vector3<T>& operator += (const Vector3<T>& v) {
		_data[0] += v._data[0];
		_data[1] += v._data[1];
		_data[2] += v._data[2];
		return(*this);
	}

	Vector3<T> operator - (const Vector3<T>& v) const {
		float x[3];
		x[0] = _data[0] - v._data[0];
		x[0] = _data[1] - v._data[1];
		x[0] = _data[2] - v._data[2];
		return(Vector3<T>(x));
	}

	Vector3<T>& operator -= (const Vector3<T>& v) {
		_data[0] -= v._data[0];
		_data[1] -= v._data[1];
		_data[2] -= v._data[2];
		return(*this);
	}

	Vector3<T> operator * (const T& v) const {
		float x[3];
		x[0] = _data[0] * v;
		x[0] = _data[1] * v;
		x[0] = _data[2] * v;
		return(Vector3<T>(x));
	}

	Vector3<T>& operator *= (const T& v) {
		_data[0] *= v;
		_data[1] *= v;
		_data[2] *= v;
		return(*this);
	}

	Vector3<T> operator / (const T& v) const {
		float x[3];
		x[0] = _data[0] / v;
		x[0] = _data[1] / v;
		x[0] = _data[2] / v;
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

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

#endif /* __VECTOR_H */