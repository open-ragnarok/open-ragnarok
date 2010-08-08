/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "ro/types/rsm.h"
#include "ro/ro.h"

#ifndef MAX
#	define MAX(x,y) (((x)>(y))?(x):(y))
#endif

#ifndef MIN
#	define MIN(x,y) (((y)>(x))?(x):(y))
#endif

namespace ro {

static RSM::Node g_emptyNode;

RSM::RSM() : Object() {
}

RSM::RSM(const RSM& rsm) : Object(rsm) {
	m_animLen = rsm.m_animLen;
	m_shadeType = rsm.m_shadeType;
	m_alpha = rsm.m_alpha;
	memcpy(m_reserved, rsm.m_reserved, sizeof(m_reserved));
	m_textures = rsm.m_textures;
	memcpy(m_mainNode, rsm.m_mainNode, sizeof(m_mainNode));
	m_nodes = rsm.m_nodes;
	m_volumeBoxes = rsm.m_volumeBoxes;
}

RSM& RSM::operator = (const RSM& rsm) {
	reset();
	rsm.copyHeader(this);
	m_animLen = rsm.m_animLen;
	m_shadeType = rsm.m_shadeType;
	m_alpha = rsm.m_alpha;
	memcpy(m_reserved, rsm.m_reserved, sizeof(m_reserved));
	m_textures = rsm.m_textures;
	memcpy(m_mainNode, rsm.m_mainNode, sizeof(m_mainNode));
	m_nodes = rsm.m_nodes;
	m_volumeBoxes = rsm.m_volumeBoxes;
	return(*this);
}

RSM::~RSM() {
	reset();
}

void RSM::reset(void) {
	m_valid = false;
	m_textures.clear();
	m_nodes.clear();
	m_volumeBoxes.clear();
}

bool RSM::readStream(std::istream& s) {
	reset();
	if (!readHeader(s)) {
		return(false);
	}

	if (!checkHeader(RSM_HEADER)) {
		_log(ROINT__DEBUG, "Invalid RSM header (%c%c%c%c)", magic[0], magic[1], magic[2], magic[3]);
		return(false);
	}

	if (m_version.cver.major == 1 && m_version.cver.minor >= 1 && m_version.cver.minor <= 5) {
		// supported [1.1 1.5]
	}
	else {
		_log(ROINT__DEBUG, "Unsupported RSM version (%u.%u)", m_version.cver.major, m_version.cver.minor);
		return(false);
	}

	int i, n;
	s.read((char*)&m_animLen, sizeof(int));
	s.read((char*)&m_shadeType, sizeof(int));
	if (IsCompatibleWith(1,4)) {
		s.read((char*)&m_alpha, sizeof(unsigned char));
	}
	else {
		m_alpha = 0xFF;
	}
	s.read(m_reserved, sizeof(m_reserved));

	// read textures
	s.read((char*)&n, sizeof(int));
	if (s.fail()) {
		reset();
		return(false);
	}
	if (n > 0) {
		m_textures.resize((unsigned int)n);
		for (i = 0; i < n; i++) {
			Texture& tex = m_textures[i];
			s.read((char*)&tex, sizeof(Texture));
			tex.name[39] = 0;
		}
	}

	// read nodes
	s.read(m_mainNode, sizeof(m_mainNode));
	s.read((char*)&n, sizeof(int));
	if (s.fail()) {
		reset();
		return(false);
	}
	if (n > 0) {
		m_nodes.resize((unsigned int)n);
		for (i = 0; i < n; i++) {
			Node& node = m_nodes[i];
		//	if (!node.readStream(s, m_version)) {
			if (!node.readStream(s, m_version, i == 0)) {
				reset();
				return(false);
			}
		}
	}
	if (m_nodes.size() == 1)
		m_nodes[0].is_only = true;

	// read position animation of main node
	if (!IsCompatibleWith(1,5)) {
		Node* mainnode = NULL;
		for (i = 0; i < (int)m_nodes.size(); i++) {
			if (strcmp(m_nodes[i].name, m_mainNode) == 0) {
				mainnode = &m_nodes[i];
				break;
			}
		}
		if (mainnode == NULL) {
			_log(ROINT__DEBUG, "RSM main node not found (%s)", m_mainNode);
			reset();
			return(false);
		}
		s.read((char*)&n, sizeof(int));
		if (s.fail()) {
			reset();
			return(false);
		}
		if (n > 0) {
			mainnode->posKeyframes.resize((unsigned int)n);
			for (i = 0; i < n; i++) {
				PosKeyframe& frame = mainnode->posKeyframes[i];
				s.read((char*)&frame, sizeof(PosKeyframe));
			}
		}
	}

	// read volume boxes
	s.read((char*)&n, sizeof(int));
	if (s.fail()) {
		reset();
		return(false);
	}
	if (n > 0) {
		m_volumeBoxes.resize((unsigned int)n);
		if (IsCompatibleWith(1,3)) {
			for (i = 0; i < n; i++) {
				VolumeBox& box = m_volumeBoxes[i];
				s.read((char*)&box, sizeof(VolumeBox));
			}
		}
		else {
			for (i = 0; i < n; i++) {
				VolumeBox& box = m_volumeBoxes[i];
				s.read((char*)&box, sizeof(VolumeBox) - sizeof(int));
				box.flag = 0;
			}
		}
	}

	calcBoundingBox();

	if (s.fail()) {
		reset();
		return(false);
	}
	m_valid = true;
	return(true);
}

bool RSM::writeStream(std::ostream& s) const {
	if (!isValid() || !writeHeader(s)) {
		return(false);
	}

	int i, n;
	s.write((char*)&m_animLen, sizeof(int));
	s.write((char*)&m_shadeType, sizeof(int));
	if (IsCompatibleWith(1,4)) {
		s.write((char*)&m_alpha, sizeof(unsigned char));
	}
	s.write(m_reserved, sizeof(m_reserved));

	// write textures
	n = (int)m_textures.size();
	s.write((char*)&n, sizeof(int));
	for (i = 0; i < n; i++) {
		const Texture& tex = m_textures[i];
		s.write((char*)&tex, sizeof(Texture));
	}

	// write nodes
	s.write(m_mainNode, sizeof(m_mainNode));
	n = (int)m_nodes.size();
	s.write((char*)&n, sizeof(int));
	for (i = 0; i < n; i++) {
		const Node& node = m_nodes[i];
		if (!node.writeStream(s, m_version)) {
			return(false);
		}
	}

	// write position animation of main node
	if (!IsCompatibleWith(1,5)) {
		const Node* mainnode = findNode(m_mainNode);
		if (mainnode == NULL) {
			_log(ROINT__DEBUG, "RSM main node not found (%s)", m_mainNode);
			return(false);
		}
		n = (int)mainnode->posKeyframes.size();
		s.write((char*)&n, sizeof(int));
		for (i = 0; i < n; i++) {
			const PosKeyframe& frame = mainnode->posKeyframes[i];
			s.write((char*)&frame, sizeof(PosKeyframe));
		}
	}

	// write volume boxes
	n = (int)m_volumeBoxes.size();
	s.write((char*)&n, sizeof(int));
	if (IsCompatibleWith(1,3)) {
		for (i = 0; i < n; i++) {
			const VolumeBox& box = m_volumeBoxes[i];
			s.write((char*)&box, sizeof(VolumeBox));
		}
	}
	else {
		for (i = 0; i < n; i++) {
			const VolumeBox& box = m_volumeBoxes[i];
			s.write((char*)&box, sizeof(VolumeBox) - sizeof(int));
		}
	}

	return(!s.fail());
}

void RSM::Dump(std::ostream& out, const std::string& prefix) const {
	char buf[16];
	unsigned int i;

	std::string subprefix = prefix + "\t";

	out << prefix << "Version: " << (short)m_version.cver.major << "." << (short)m_version.cver.minor << std::endl;
	out << prefix << "Anim len: " << m_animLen << std::endl;
	out << prefix << "Shade type: " << m_shadeType << std::endl;
	out << prefix << "Alpha: " << (short)m_alpha << std::endl;
	out << prefix << "Reserved: ";
	for (i = 0; i < 25; i++) {
		sprintf(buf, "0x%x ", (unsigned char)m_reserved[i]);
		out << buf;
	}
	out << std::endl;
	out << prefix << "Main node: " << m_mainNode << std::endl;

	out << prefix << "Textures: (" << m_textures.size() << ")" << std::endl;
	for (i = 0; i < m_textures.size(); i++) {
		const Texture& tex = m_textures[i];
		out << subprefix << "[" << i << "]" << std::endl;
		out << subprefix << tex.name << std::endl;
	}

	out << prefix << "Nodes: (" << m_nodes.size() << ")" << std::endl;
	for (i = 0; i < m_nodes.size(); i++) {
		const Node& node = m_nodes[i];
		out << subprefix << "[" << i << "]" << std::endl;
		node.Dump(out, subprefix);
	}

	out << prefix << "Volume boxes: (" << m_volumeBoxes.size() << ")" << std::endl;
	for (i = 0; i < m_volumeBoxes.size(); i++) {
		const VolumeBox& box = m_volumeBoxes[i];
		out << subprefix << "[" << i << "]" << std::endl;
		out << subprefix << "Size: [ " << box.size.x << " " << box.size.y << " " << box.size.z << " ]" << std::endl;
		out << subprefix << "Pos: [ " << box.pos.x << " " << box.pos.y << " " << box.pos.z << " ]" << std::endl;
		out << subprefix << "Rot: [ " << box.rot.x << " " << box.rot.y << " " << box.rot.z << " ]" << std::endl;
	}
}

void ro::RSM::calcBoundingBox() {
#if 1
	unsigned int i;

	m_nodes[0].calcBoundingBox();
	for (i = 1; i < m_nodes.size(); i++)
		if (strcmp(m_nodes[i].parentname, m_nodes[0].name) == 0)
			m_nodes[i].calcBoundingBox();

	for (i = 0; i < 3; i++) {
		box.max[i] = m_nodes[0].getBoundingBox().max[i];
		box.min[i] = m_nodes[0].getBoundingBox().min[i];
	/*	for (unsigned int j = 1; j < m_nodes.size(); j++) {
			if (strcmp(m_nodes[i].parentname, m_nodes[0].name) == 0){
				box.max[i] = MAX(m_nodes[j].getBoundingBox().max[i], box.max[i]);
				box.min[i] = MIN(m_nodes[j].getBoundingBox().min[i], box.min[i]);
			}
		}*/
		box.offset[i] = (box.max[i] + box.min[i]) / 2.0f;
		box.range[i] = (box.max[i] - box.min[i]) / 2.0f;
	}
#else
	unsigned int i, j;
	BoundingBox& meshbox = box;
	box.max.x = -999999;
	box.max.y = -999999;
	box.max.z = -999999;

	box.min.x = 999999;
	box.min.y = 999999;
	box.min.z = 999999;

	box.range.x = 0;
	box.range.y = 0;
	box.range.z = 0;
	for (i = 0; i < m_nodes.size(); i++) {
		m_nodes[i].calcBoundingBox(m_nodes[0].transf);
		meshbox = m_nodes[i].getBoundingBox();

		for(j = 0; j < 3; j++) {
			if (meshbox.max[j] > box.max[j])
				box.max[j] = meshbox.max[j];
			if (meshbox.min[j] < box.min[j])
				box.min[j] = meshbox.min[j];
		}
	}

	for(j = 0; j < 3; j++)
		// box.range[j] = box.max[j] - box.min[j];
		box.range[j] = (box.max[j] + box.min[j])/2.0f;
#endif
}

const ro::RSM::BoundingBox& ro::RSM::getBoundingBox() const {
	return(box);
}

int RSM::getAnimLen() const {
	return(m_animLen);
}

int RSM::getShadeType() const {
	return(m_shadeType);
}

unsigned char RSM::getAlpha() const {
	return(m_alpha);
}

const char* RSM::getMainNode() const {
	return(m_mainNode);
}

unsigned int RSM::getNodeCount() const {
	return(m_nodes.size());
}

const RSM::Node& RSM::getNode(unsigned int idx) const {
	if (idx < m_nodes.size())
		return(m_nodes[idx]);
	return(g_emptyNode);
}

const RSM::Node& RSM::operator[] (unsigned int idx) const {
	return(m_nodes[idx]);
}

const RSM::Node* RSM::findNode(const char* name) const {
	for (unsigned int i = 0; i < m_nodes.size(); i++) {
		const Node& node = m_nodes[i];
		if (strcmp(name, node.name) == 0)
			return(&node);
	}
	return(NULL);
}

unsigned int RSM::getTextureCount() const {
	return(m_textures.size());
}

const char* RSM::getTexture(unsigned int idx) const {
	if (idx < m_textures.size())
		return(m_textures[idx].name);
	return("");
}

RSM::Node::Node() {
    is_main = true;
    is_only = false;
}

RSM::Node::Node(const RSM::Node& node) {
	*this = node;
}

RSM::Node::~Node() {
	reset();
}

RSM::Node& RSM::Node::operator = (const RSM::Node& node) {
	memcpy((char*)&name, (char*)&node.name, sizeof(name));
	memcpy((char*)&parentname, (char*)&node.parentname, sizeof(parentname));
	textures = node.textures;
    memcpy(offsetMT, node.offsetMT, sizeof(offsetMT));
	pos = node.pos;
	rotangle = node.rotangle;
	rotaxis = node.rotaxis;
	scale = node.scale;
	vertices = node.vertices;
	tvertices = node.tvertices;
	faces = node.faces;
	posKeyframes = node.posKeyframes;
	rotKeyframes = node.rotKeyframes;
	is_main = node.is_main;
	is_only = node.is_only;
	return(*this);
}

unsigned int RSM::getVolumeBoxCount() const {
	return m_volumeBoxes.size();
}

const RSM::VolumeBox& RSM::getVolumeBox(unsigned int idx) const {
	return m_volumeBoxes[idx];
}

void RSM::Node::reset() {
	memset(name, 0, sizeof(name));
	memset(parentname, 0, sizeof(parentname));
    textures.clear();
    memset((char*)&offsetMT, 0, sizeof(offsetMT));
	memset((char*)&pos, 0, sizeof(pos));
	rotangle = 0;
	memset((char*)&rotaxis, 0, sizeof(rotaxis));
	scale.x = scale.y = scale.z = 1;
    vertices.clear();
    tvertices.clear();
    faces.clear();
	posKeyframes.clear();
    rotKeyframes.clear();
}

//bool RSM::Node::readStream(std::istream& s, const s_obj_ver& ver) {
bool RSM::Node::readStream(std::istream& s, const s_obj_ver& ver, bool main) {
	reset();
	s.read(name, 40);
	s.read(parentname, 40);

	is_main = main;

	int i, n;
	// read texture indexes
	s.read((char*)&n, sizeof(int));
	if (s.fail())
		return(false);
	if (n > 0) {
		textures.resize((unsigned int)n);
		for (i = 0; i < n; i++) {
			int& index = textures[i];
			s.read((char*)&index, sizeof(int));
		}
	}

	s.read((char*)&offsetMT, sizeof(offsetMT));
	s.read((char*)&pos, sizeof(pos));
	s.read((char*)&rotangle, sizeof(float));
	s.read((char*)&rotaxis, sizeof(rotaxis));
	s.read((char*)&scale, sizeof(scale));

	// read vertex indexes
	s.read((char*)&n, sizeof(int));
	if (s.fail())
		return(false);
	if (n > 0) {
		vertices.resize((unsigned int)n);
		for (i = 0; i < n; i++) {
			Vertex& vert = vertices[i];
			s.read((char*)&vert, sizeof(Vertex));
		}
	}

	// read texture vertex indexes
	s.read((char*)&n, sizeof(int));
	if (s.fail())
		return(false);
	if (n > 0) {
		tvertices.resize((unsigned int)n);
		if (ver.cver.major > 1 || (ver.cver.major == 1 && ver.cver.minor >= 2)) {
			for (i = 0; i < n; i++) {
				TVertex& tvert = tvertices[i];
				s.read((char*)&tvert, sizeof(TVertex));
			}
		} else {
			for (i = 0; i < n; i++) {
				TVertex& tvert = tvertices[i];
				tvert.color = 0xFFFFFFFF;
				s.read((char*)&tvert.u, sizeof(float));
				s.read((char*)&tvert.v, sizeof(float));
			}
		}
	}

	// read faces
	s.read((char*)&n, sizeof(int));
	if (s.fail())
		return(false);
	if (n > 0) {
		faces.resize((unsigned int)n);
		if (ver.cver.major > 1 || (ver.cver.major == 1 && ver.cver.minor >= 2)) {
			for (i = 0; i < n; i++) {
				Face& face = faces[i];
				s.read((char*)&face, sizeof(Face));
			}
		} else {
			for (i = 0; i < n; i++) {
				Face& face = faces[i];
				s.read((char*)&face, sizeof(Face) - sizeof(int));
				face.smoothGroup = 0;
			}
		}
	}

	// read position keyframes
	if (ver.cver.major > 1 || (ver.cver.major == 1 && ver.cver.minor >= 5)) {
		s.read((char*)&n, sizeof(int));
		if (s.fail())
			return(false);
		if (n > 0) {
			posKeyframes.resize((unsigned int)n);
			for (i = 0; i < n; i++) {
				PosKeyframe& frame = posKeyframes[i];
				s.read((char*)&frame, sizeof(PosKeyframe));
			}
		}
	}

	// read rotation keyframes
	s.read((char*)&n, sizeof(int));
	if (s.fail())
		return(false);
	if (n > 0) {
		rotKeyframes.resize((unsigned int)n);
		for (i = 0; i < n; i++) {
			RotKeyframe& frame = rotKeyframes[i];
			s.read((char*)&frame, sizeof(RotKeyframe));
		}
	}

	return(!s.fail());
}

bool RSM::Node::writeStream(std::ostream& s, const s_obj_ver& ver) const {
	s.write(name, 40);
	s.write(parentname, 40);

	int i, n;
	// write texture indexes
	n = (int)textures.size();
	s.write((char*)&n, sizeof(int));
	for (i = 0; i < n; i++) {
		const int& index = textures[i];
		s.write((char*)&index, sizeof(int));
	}

	s.write((char*)&offsetMT, sizeof(offsetMT));
	s.write((char*)&pos, sizeof(pos));
	s.write((char*)&rotangle, sizeof(float));
	s.write((char*)&rotaxis, sizeof(rotaxis));
	s.write((char*)&scale, sizeof(scale));

	// write vertex indexes
	n = (int)vertices.size();
	s.write((char*)&n, sizeof(int));
	for (i = 0; i < n; i++) {
		const Vertex& vert = vertices[i];
		s.write((char*)&vert, sizeof(Vertex));
	}

	// write texture vertex indexes
	n = (int)tvertices.size();
	s.write((char*)&n, sizeof(int));
	if (ver.cver.major > 1 || (ver.cver.major == 1 && ver.cver.minor >= 2)) {
		for (i = 0; i < n; i++) {
			const TVertex& tvert = tvertices[i];
			s.write((char*)&tvert, sizeof(TVertex));
		}
	} else {
		for (i = 0; i < n; i++) {
			const TVertex& tvert = tvertices[i];
			s.write((char*)&tvert.u, sizeof(float));
			s.write((char*)&tvert.v, sizeof(float));
		}
	}

	// write faces
	n = (int)faces.size();
	s.write((char*)&n, sizeof(int));
	if (ver.cver.major > 1 || (ver.cver.major == 1 && ver.cver.minor >= 2)) {
		for (i = 0; i < n; i++) {
			const Face& face = faces[i];
			s.write((char*)&face, sizeof(Face));
		}
	} else {
		for (i = 0; i < n; i++) {
			const Face& face = faces[i];
			s.write((char*)&face, sizeof(Face) - sizeof(int));
		}
	}

	// write position keyframes
	if (ver.cver.major > 1 || (ver.cver.major == 1 && ver.cver.minor >= 5)) {
		n = (int)posKeyframes.size();
		s.write((char*)&n, sizeof(int));
		for (i = 0; i < n; i++) {
			const PosKeyframe& frame = posKeyframes[i];
			s.write((char*)&frame, sizeof(PosKeyframe));
		}
	}

	// write rotation keyframes
	n = (int)rotKeyframes.size();
	s.write((char*)&n, sizeof(int));
	for (i = 0; i < n; i++) {
		const RotKeyframe& frame = rotKeyframes[i];
		s.write((char*)&frame, sizeof(RotKeyframe));
	}

	return(!s.fail());
}

void RSM::Node::Dump(std::ostream& out, const std::string& prefix) const {
	char buf[512];
	unsigned int i;

	std::string subprefix = prefix + "\t";

	out << prefix << "Name: " << name << std::endl;
	out << prefix << "Parent name: " << parentname << std::endl;

	out << prefix << "Offset matrix: (3x4)" << std::endl;
	sprintf(buf, "[ %.5f %.5f %.5f ]", offsetMT[0], offsetMT[1], offsetMT[2]);
	out << subprefix << buf << std::endl;
	sprintf(buf, "[ %.5f %.5f %.5f ]", offsetMT[3], offsetMT[4], offsetMT[5]);
	out << subprefix << buf << std::endl;
	sprintf(buf, "[ %.5f %.5f %.5f ]", offsetMT[6], offsetMT[7], offsetMT[8]);
	out << subprefix << buf << std::endl;
	sprintf(buf, "[ %.5f %.5f %.5f ]", offsetMT[9], offsetMT[10], offsetMT[11]);
	out << subprefix << buf << std::endl;

	sprintf(buf, "Pos: [ %.2f %.2f %.2f ]", pos.x, pos.y, pos.z);
	out << buf << std::endl;

	sprintf(buf, "Rotation: %.2f [ %.2f %.2f %.2f ]", rotangle, rotaxis.x, rotaxis.y, rotaxis.z);
	out << prefix << buf << std::endl;
	sprintf(buf, "Scale: [ %.2f %.2f %.2f ]", scale.x, scale.y, scale.z);
	out << prefix << buf << std::endl;

	out << prefix << "Textures: (" << textures.size() << ")" << std::endl;
	for (i = 0; i < textures.size(); i++) {
		const int& texidx = textures[i];
		out << subprefix << texidx << std::endl;
	}

	out << prefix << "Vertices: (" << vertices.size() << ")" << std::endl;
	for (i = 0; i < vertices.size(); i++) {
		const Vertex& vert = vertices[i];
		sprintf(buf, "[ %.2f %.2f %.2f ]", vert.x, vert.y, vert.z);
		out << subprefix << buf << std::endl;
	}

	out << prefix << "Texture vertices: (" << tvertices.size() << ")" << std::endl;
	for (i = 0; i < tvertices.size(); i++) {
		const TVertex& tvert = tvertices[i];
		sprintf(buf, "[ 0x%8x %.2f %.2f ]", tvert.color, tvert.u, tvert.v);
		out << subprefix << buf << std::endl;
	}

	out << prefix << "Faces: (" << faces.size() << ")" << std::endl;
	for (i = 0; i < faces.size(); i++) {
		const Face& face = faces[i];
		out << subprefix << "[" << i << "]" << std::endl;
		out << subprefix << "Vert indexes: " << face.vertidx[0] << ", " << face.vertidx[1] << ", " << face.vertidx[2] << std::endl;
		out << subprefix << "Texture vert indexes: " << face.tvertidx[0] << ", " << face.tvertidx[1] << ", " << face.tvertidx[2] << std::endl;
		out << subprefix << "Texture id: " << face.texid << std::endl;
		out << subprefix << "Two side: " << face.twoSide << std::endl;
		out << subprefix << "Smooth group: " << face.smoothGroup << std::endl;
	}

	out << prefix << "Position Keyframes: (" << posKeyframes.size() << ")" << std::endl;
	for (i = 0; i < posKeyframes.size(); i++) {
		const PosKeyframe& frame = posKeyframes[i];
		sprintf(buf, "%d [ %.2f %.2f %.2f ]", frame.frame, frame.px, frame.py, frame.pz);
		out << subprefix << buf << std::endl;
	}

	out << prefix << "Rotation Keyframes: (" << rotKeyframes.size() << ")" << std::endl;
	for (i = 0; i < rotKeyframes.size(); i++) {
		const RotKeyframe& frame = rotKeyframes[i];
		sprintf(buf, "%d [ %.2f %.2f %.2f %.2f ]", frame.frame, frame.qx, frame.qy, frame.qz, frame.qw);
		out << subprefix << buf << std::endl;
	}

}

void MatrixMultVect(const float* M, const RSM::Vertex& Vin, RSM::Vertex& Vout) {
	Vout[0] = Vin[0]*M[0] + Vin[1]*M[4] + Vin[2]*M[8] + 1.0f * M[12];
	Vout[1] = Vin[0]*M[1] + Vin[1]*M[5] + Vin[2]*M[9] + 1.0f * M[13];
	Vout[2] = Vin[0]*M[2] + Vin[1]*M[6] + Vin[2]*M[10] + 1.0f * M[14];
}

void ro::RSM::Node::calcBoundingBox() {
	box.max[0] = box.max[1] = box.max[2] = -999999.0;
	box.min[0] = box.min[1] = box.min[2] = 999999.0;

	float transf[15];
	transf[0] = offsetMT[0];
	transf[1] = offsetMT[1];
	transf[2] = offsetMT[2];
	transf[3] = 0;

	transf[4] = offsetMT[3];
	transf[5] = offsetMT[4];
	transf[6] = offsetMT[5];
	transf[7] = 0;

	transf[8] = offsetMT[6];
	transf[9] = offsetMT[7];
	transf[10] = offsetMT[8];
	transf[11] = 0;

	transf[12] = 0;
	transf[13] = 0;
	transf[14] = 0;
	transf[15] = 1;

	unsigned int i, j;
	Vertex vi, v;
#if 1
	for (i = 0; i < vertices.size(); i++) {
		MatrixMultVect(transf, vertices[i], v);

		for (j = 0; j < 3; j++) {
			if (!is_only) v[j] += offsetMT[9 + j];// + t.translate2.v[j];

			box.min[j] = MIN(v[j], box.min[j]);
			box.max[j] = MAX(v[j], box.max[j]);
		}
	}
#else
	for (i = 0; i < vertices.size(); i++) {
		for (j = 0; j < 3; j++) {
			if (!is_only) v[j] += offsetMT[9 + j];// + t.translate2.v[j];

			box.min[j] = MIN(vertices[i][j], box.min[j]);
			box.max[j] = MAX(vertices[i][j], box.max[j]);
		}
	}

	MatrixMultVect(transf, box.max, v);
	box.max = v;
	MatrixMultVect(transf, box.min, v);
	box.min = v;
#endif
	for (i = 0; i < 3; i++) {
		box.offset[i] = (box.max[i] + box.min[i]) / 2.0f;
		box.range[i] = (box.max[i] - box.min[i]) / 2.0f;
	}
}

const ro::RSM::BoundingBox& ro::RSM::Node::getBoundingBox() const {
	return(box);
}

#ifdef ROINT_USE_XML

TiXmlElement* RSM::GenerateXML(const std::string& name, bool utf) const {
	char buf[256];
	unsigned int i, k, m;
	TiXmlElement* root = new TiXmlElement("RSM");
	TiXmlElement* child = NULL;

	sprintf(buf,"%d.%d", m_version.cver.major , m_version.cver.minor);
	root->SetAttribute("version", buf);
	if (name != "") {
		root->SetAttribute("name", name);
	}

	root->SetAttribute("animlen", m_animLen);
	root->SetAttribute("shadetype", m_shadeType);
	root->SetAttribute("alpha", (int)m_alpha);
	sprintf(buf, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
		m_reserved[0], m_reserved[1], m_reserved[ 2], m_reserved[ 3], m_reserved[ 4], m_reserved[ 5], m_reserved[ 6], m_reserved[ 7],
		m_reserved[8], m_reserved[9], m_reserved[10], m_reserved[11], m_reserved[12], m_reserved[13], m_reserved[14], m_reserved[15]);
	root->SetAttribute("reserved", buf);
	root->SetAttribute("mainnode", m_mainNode);

	// textures
	for (i = 0 ; i < m_textures.size(); i++) {
		std::string texname = m_textures[i].name;
		if (utf) {
			texname = euc2utf8(texname);
		}
		TiXmlElement *texture = new TiXmlElement("texture");
		texture->SetAttribute("id", i);
		texture->LinkEndChild(new TiXmlText(texname));
		root->LinkEndChild(texture);
	}

	// nodes
	for(i = 0; i < m_nodes.size(); i++) {
		const Node& node = m_nodes[i];
		char buf[16];
		TiXmlElement* nodexml = new TiXmlElement("node");
		nodexml->SetAttribute("id", i);
		nodexml->SetAttribute("name", node.name);
		if (strlen(node.parentname) > 0) {
			nodexml->SetAttribute("parentname", node.parentname);
		}
		TiXmlElement* matrixxml = new TiXmlElement("offsetmatrix");
		TiXmlElement* childxml = new TiXmlElement("col");
		sprintf(buf, "%f", node.offsetMT[0]);
		childxml->SetAttribute("x", buf);
		sprintf(buf, "%f", node.offsetMT[1]);
		childxml->SetAttribute("y", buf);
		sprintf(buf, "%f", node.offsetMT[2]);
		childxml->SetAttribute("z", buf);
		matrixxml->LinkEndChild(childxml);

		childxml = new TiXmlElement("col");
		sprintf(buf, "%f", node.offsetMT[3]);
		childxml->SetAttribute("x", buf);
		sprintf(buf, "%f", node.offsetMT[4]);
		childxml->SetAttribute("y", buf);
		sprintf(buf, "%f", node.offsetMT[5]);
		childxml->SetAttribute("z", buf);
		matrixxml->LinkEndChild(childxml);

		childxml = new TiXmlElement("col");
		sprintf(buf, "%f", node.offsetMT[6]);
		childxml->SetAttribute("x", buf);
		sprintf(buf, "%f", node.offsetMT[7]);
		childxml->SetAttribute("y", buf);
		sprintf(buf, "%f", node.offsetMT[8]);
		childxml->SetAttribute("z", buf);
		matrixxml->LinkEndChild(childxml);

		childxml = new TiXmlElement("col");
		sprintf(buf, "%f", node.offsetMT[9]);
		childxml->SetAttribute("x", buf);
		sprintf(buf, "%f", node.offsetMT[10]);
		childxml->SetAttribute("y", buf);
		sprintf(buf, "%f", node.offsetMT[11]);
		childxml->SetAttribute("z", buf);
		matrixxml->LinkEndChild(childxml);

		nodexml->LinkEndChild(matrixxml);


		childxml = new TiXmlElement("pos");
		sprintf(buf, "%f", node.pos.x);
		childxml->SetAttribute("x", buf);
		sprintf(buf, "%f", node.pos.y);
		childxml->SetAttribute("y", buf);
		sprintf(buf, "%f", node.pos.z);
		childxml->SetAttribute("z", buf);
		nodexml->LinkEndChild(childxml);

		childxml = new TiXmlElement("rotation");
		sprintf(buf, "%f", node.rotangle);
		childxml->SetAttribute("angle", buf);
		sprintf(buf, "%f", node.rotaxis.x);
		childxml->SetAttribute("x", buf);
		sprintf(buf, "%f", node.rotaxis.y);
		childxml->SetAttribute("y", buf);
		sprintf(buf, "%f", node.rotaxis.z);
		childxml->SetAttribute("z", buf);
		nodexml->LinkEndChild(childxml);

		childxml = new TiXmlElement("scale");
		sprintf(buf, "%f", node.scale.x);
		childxml->SetAttribute("x", buf);
		sprintf(buf, "%f", node.scale.y);
		childxml->SetAttribute("y", buf);
		sprintf(buf, "%f", node.scale.z);
		childxml->SetAttribute("z", buf);
		nodexml->LinkEndChild(childxml);

		// vertices
		for (k = 0; k < node.vertices.size(); k++) {
			const Vertex& vert = node.vertices[k];
			TiXmlElement* vertxml = new TiXmlElement("vertice");
			sprintf(buf, "%f", vert.x);
			vertxml->SetAttribute("x", buf);
			sprintf(buf, "%f", vert.y);
			vertxml->SetAttribute("y", buf);
			sprintf(buf, "%f", vert.z);
			vertxml->SetAttribute("z", buf);
			nodexml->LinkEndChild(vertxml);
		}

		// texture vertices
		for (k = 0; k < node.tvertices.size(); k++) {
			const TVertex& tvert = node.tvertices[k];
			TiXmlElement* tvertxml = new TiXmlElement("vertice");
			sprintf(buf, "0x%08x", tvert.color);
			tvertxml->SetAttribute("color", buf);
			sprintf(buf, "%f", tvert.u);
			tvertxml->SetAttribute("u", buf);
			sprintf(buf, "%f", tvert.v);
			tvertxml->SetAttribute("v", buf);
			nodexml->LinkEndChild(tvertxml);
		}

		// faces
		for (k = 0; k < node.faces.size(); k++) {
			const Face& face = node.faces[k];
			TiXmlElement* facexml = new TiXmlElement("face");
			
			for (m = 0; m < 3; m++) {
				TiXmlElement* vertxml = new TiXmlElement("vertice");
				vertxml->SetAttribute("vertidx", (int)face.vertidx[m]);
				vertxml->SetAttribute("tvertidx", (int)face.tvertidx[m]);
				facexml->LinkEndChild(vertxml);
			}
			facexml->SetAttribute("texid", (int)face.texid);
			facexml->SetAttribute("twoside", (int)face.twoSide);
			facexml->SetAttribute("smoothgroup", (int)face.smoothGroup);
			nodexml->LinkEndChild(facexml);
		}

		// position keyframes
		for (k = 0; k < node.posKeyframes.size(); k++) {
			const PosKeyframe& frame = node.posKeyframes[k];
			TiXmlElement* framexml = new TiXmlElement("poskeyframe");
			framexml->SetAttribute("frame", frame.frame);
			sprintf(buf, "%f", frame.px);
			framexml->SetAttribute("px", buf);
			sprintf(buf, "%f", frame.py);
			framexml->SetAttribute("py", buf);
			sprintf(buf, "%f", frame.pz);
			framexml->SetAttribute("pz", buf);
			nodexml->LinkEndChild(framexml);
		}

		// rotation keyframes
		for (k = 0; k < node.rotKeyframes.size(); k++) {
			const RotKeyframe& frame = node.rotKeyframes[k];
			TiXmlElement* framexml = new TiXmlElement("rotkeyframe");
			framexml->SetAttribute("frame", frame.frame);
			sprintf(buf, "%f", frame.qx);
			framexml->SetAttribute("qx", buf);
			sprintf(buf, "%f", frame.qy);
			framexml->SetAttribute("qy", buf);
			sprintf(buf, "%f", frame.qz);
			framexml->SetAttribute("qz", buf);
			sprintf(buf, "%f", frame.qw);
			framexml->SetAttribute("qw", buf);
			nodexml->LinkEndChild(framexml);
		}

		root->LinkEndChild(nodexml);
	}

	return(root);
}

const float* ro::RSM::getColor() const {
	static float color[4] = {1,1,1,1};
	if (IsCompatibleWith(1,5)) {
		color[1] = color[2] = 0;
	}
	else if (IsCompatibleWith(1,4)) {
		color[0] = color[2] = 0;
	}
	else if (IsCompatibleWith(1,3)) {
		color[0] = color[1] = 0;
	}

	return color;
}

#endif

} /* namespace ro */
