/* $id$ */
#ifndef __RSM_H
#define __RSM_H

#include "object.h"
#include "struct_io.h"

namespace RO {
	class MYLIB_DLLAPI RSM : public Object {
	public:
		// RSM Internal Structures
#pragma pack(push)
#pragma pack(1)
		typedef struct SprSize {
		  int width;
		  int height;
		  int tex_w;
		  int tex_h;
		};

		typedef char TexName[40];

		union Vec {
			struct {
				float x, y, z;
			} c;
			float v[3];
		};

		typedef struct Surface {
			unsigned short sv[3];
			unsigned short tv[3];
			unsigned short texid;
			unsigned short unk1;
			unsigned int unk2;
			unsigned int nsurf;
		};

		typedef struct Frame {
			int time;
			float orientation[4];
		};

		typedef struct BoundingBox {
			Vec max;
			Vec min;
			Vec range;
		};


		class Mesh {
		public:
			struct Header {
			  char name[40];
			  int unk1;
			  char parent[40];
			  float unk2[10];
			};

			struct Transf {
			  float mat33[9];
			  Vec translate1;
			  Vec translate2;
			  float rot_angle;
			  Vec rot_vector;
			  Vec scale;
			};

			/* This happens only if there are no frames
			  glRotatef(transf.rot_angle * 180.0 / PI,
					transf.rot_vector.x, transf.rot_vector.y, transf.rot_vector.z);

			*/

			struct Frame {
			  int time;
			  float rot[4];
			};

			bool is_main;
			bool is_only;

			Header header;
			Transf transf;

			StructIO<int> textures;
			StructIO<Vec> vecs;
			StructIO<Vec> texv;
			StructIO<Surface> surfaces;
			StructIO<Frame> frames;

			BoundingBox box;

			Mesh();
			Mesh(const Mesh&);
			~Mesh();
			void Clear();
			void Write(std::ostream& s) const;
			bool readStream(std::istream& s, bool main = true);
			void calcBoundingBox();

			void Dump(std::ostream& out, const std::string& prefix) const;
			Mesh& operator = (const Mesh&);
			Mesh& operator = (const Mesh*);
		};
#pragma pack(pop)

	protected:
		char garbage[25];
		unsigned int meshCount;
		StructIO<TexName> m_textures;
		Mesh* m_meshes;

	public:
		RSM();
		virtual ~RSM();

		bool Write(std::ostream& s) const;
		virtual bool readStream(std::istream&);
		void Dump(std::ostream& out, const std::string& = "") const;

		unsigned int getMeshCount() const;
		unsigned int getTextureCount() const;

		Mesh& operator[] (const unsigned int&);
		char* getTexture(const unsigned int&);

		const Mesh& operator[] (const unsigned int&) const;
		const char* getTexture(const unsigned int&) const;

		Mesh& getMesh(const unsigned int&);
		const Mesh& getMesh(const unsigned int&) const;
	};
}

#endif /* __RSM_H */
