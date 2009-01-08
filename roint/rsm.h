/* $Id$ */
#ifndef __RSM_H
#define __RSM_H

#include "object.h"
#include "struct_io.h"

namespace RO {
	/**
	 * Resource Model.
	 *
	 * Holds information about 3D objects
	 *
	 * \ingroup ROInterface
	 */
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


		class MYLIB_DLLAPI Mesh {
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

			const BoundingBox& getBoundingBox() const;

			Mesh();
			Mesh(const Mesh&);
			~Mesh();
			void Clear();
			void Write(std::ostream& s) const;
			bool readStream(std::istream& s, bool main = true);
			void calcBoundingBox(const RO::RSM::Mesh::Transf&);

			void Dump(std::ostream& out, const std::string& prefix) const;
			Mesh& operator = (const Mesh&);
			Mesh& operator = (const Mesh*);

			unsigned int getFrameCount() const;
			const Frame& getFrame(const unsigned int&) const;
			Frame& getFrame(const unsigned int&);

		protected:
			BoundingBox box;
		};
#pragma pack(pop)

	protected:
		/** Unknown information included in all RSM */
		char garbage[25];
		unsigned int meshCount;
		StructIO<TexName> m_textures;
		Mesh* m_meshes;

		void calcBoundingBox();
		BoundingBox box;

	public:
		RSM();
		RSM(const RSM&);
		virtual ~RSM();

		const BoundingBox& getBoundingBox() const;

		bool Write(std::ostream& s) const;
		virtual bool readStream(std::istream&);
		void Dump(std::ostream& out, const std::string& = "") const;

#ifdef ROINT_USE_XML
		TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;
		TiXmlDocument GenerateXMLDoc(const std::string& name = "", bool utf = true) const;
		bool SaveXML(std::ostream& out, const std::string& name = "", bool utf = true) const;
		bool SaveXML(const std::string& fn, const std::string& name = "", bool utf = true) const;
#endif

		unsigned int getMeshCount() const;
		unsigned int getTextureCount() const;

		Mesh& operator[] (const unsigned int&);
		char* getTexture(const unsigned int&);

		const Mesh& operator[] (const unsigned int&) const;
		const char* getTexture(const unsigned int&) const;

		Mesh& getMesh(const unsigned int&);
		const Mesh& getMesh(const unsigned int&) const;

		RSM& operator = (const RSM&);
	};
}

#endif /* __RSM_H */

