/* $Id$ */
#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "image.h"
#include <list>

class Texture {
private:
	std::string name;
	unsigned int texid;
	long refcount;

	int m_width, m_height;
	float m_maxu, m_maxv;

	static std::list<Texture*> objects;

public:
	Texture();
	Texture(const Image&, const std::string& = "");
	Texture(const Texture&);
	Texture(const unsigned int&, const std::string& = "");
	~Texture();

	void AddRef();
	void Release();

	Texture& operator = (const Texture&);
	Texture& operator = (unsigned int);

	static void CollectGarbage();

	unsigned int operator*() const;
	unsigned int getIdx() const;

	void Activate() const;

	int getWidth() const;
	int getHeight() const;

	float getMaxU() const;
	float getMaxV() const;

	class Pointer {
	private:
		Texture* texture;
	public:
		Pointer();
		Pointer(Texture*);
		Pointer(const Pointer&);
		~Pointer();

		void setTexture(Texture*);

		Pointer& operator = (Texture*);
		Pointer& operator = (const Pointer&);

		unsigned int operator *() const;
		const Texture* operator ->() const;

		void Activate() const;
		bool isValid() const;
	};

	class PointerCache {
	protected:
		std::vector<Pointer> pointers;
	public:
		PointerCache();
		PointerCache(const PointerCache&);
		~PointerCache();

		Pointer& operator[] (const unsigned int&);
		const Pointer& operator[] (const unsigned int&) const;

		void add(const Pointer&);
		void clear();
		unsigned int size() const;

		PointerCache& operator = (const PointerCache&);
	};
};


#endif /* __TEXTURE_H */