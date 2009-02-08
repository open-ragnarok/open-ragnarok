/* $Id$ */
#ifndef __TEXTUREMANAGER_H
#define __TEXTUREMANAGER_H

#include "file_manager.h"

#include <string>
#include <map>
#include <list>

class Texture {
private:
	std::string name;
	unsigned int texid;
	long refcount;

	static std::list<Texture*> objects;

public:
	Texture();
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

	class Pointer {
	private:
		Texture* texture;
	public:
		Pointer();
		Pointer(Texture*);
		Pointer(Pointer&);
		~Pointer();

		void setTexture(Texture*);

		Pointer& operator = (Texture*);
		Pointer& operator = (Pointer&);

		unsigned int operator *() const;

		void Activate() const;
	};

	class PointerCache {
	protected:
		std::vector<Pointer*> pointers;
	public:
		PointerCache();
		~PointerCache();

		Pointer* operator[] (const unsigned int&);
		const Pointer* operator[] (const unsigned int&) const;

		void add(Pointer*);
		void clear();
	};
};

class TextureManager {
protected:
	typedef std::map<std::string, Texture*> tex_t;
	tex_t textures;
public:
	TextureManager();
	~TextureManager();

	void Clear();

	Texture::Pointer Register(FileManager&, const std::string& name);
	bool UnRegister(const std::string& name);
	bool IsRegistered(const std::string& name) const;
	bool Activate(const std::string& name) const;

	Texture::Pointer operator [](const std::string& name) const;
};

#endif /* __TEXTUREMANAGER_H */