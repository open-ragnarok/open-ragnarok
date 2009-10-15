# Copyright (C) 2008 Open-Ragnarok Project
# http://www.open-ragnarok.org
# $Id$
from ctypes import *
import ro

class RSM_SprSize(Structure):
  _pack = 1
  _fields_ = [
    ("width", c_int),
    ("height", c_int),
    ("tex_w", c_int),
    ("tex_h", c_int)
  ]
  
class RSM_VecCoord(Structure):
  _pack = 1
  _fields_ = [
    ("x", c_float),
    ("y", c_float),
    ("z", c_float)
  ]
  
class RSM_Vec(Union):
  _fields_ = [
    ("c", RSM_VecCoord),
    ("f", c_float * 3)
  ]
  
  def Dump(self):
    print("%.2f %.2f %.2f" % (self.f[0], self.f[1], self.f[2]))
  
class RSM_Mesh_Header(Structure):
  _pack = 1
  _fields_ = [
    ("name",	POINTER(c_char)), # char[40]
    ("unk1",	c_int),
    ("parent",	POINTER(c_char)), # char[40]
    ("unk2",	c_float)
  ]

class RSM_Mesh_Transf(Structure):
  _pack = 1
  _fields_ = [
    ("mat33", c_float * 9),
    ("translate1", RSM_Vec),
    ("translate2", RSM_Vec), 
    ("rot_angle", c_float),
    ("rot_vector", RSM_Vec),
    ("scale", RSM_Vec)
  ]

class RSM_Mesh_Surface(Structure):
  _pack_ = 1
  _fields_ = [
    ("sv", c_short * 3),
    ("tv", c_short * 3),
    ("texid", c_short),
    ("unk1", c_short),
    ("unk2", c_int),
    ("nsurf", c_int)
  ]
  
  def Dump(self):
    print("vec %d %d %d" % (self.sv[0], self.sv[1], self.sv[2]))
    print("tex %d %d %d" % (self.tv[0], self.tv[1], self.tv[2]))
    print("texid %d" % self.texid)

class RSM:
	__roint = None
	_base = None
	def __init__(self):
		self.__roint = ro.Int()
		self._base = self.__roint.new_RSM() # store C++ superclass obj
		self._getTexture = self.__roint.getTexture_RSM
		self._getTexture.restype = c_char_p
		
		self._getMeshSurf = self.__roint.getMeshSurf_RSM
		self._getMeshSurf.restype = POINTER(RSM_Mesh_Surface)
		
		self._getMeshVec = self.__roint.getMeshVec_RSM
		self._getMeshVec.restype = POINTER(RSM_Vec)
		
		self._getMeshTexv = self.__roint.getMeshTexv_RSM
		self._getMeshTexv.restype = POINTER(RSM_Vec)
		
  
	def __del__(self):
		self.__roint.del_RSM(self._base)
 
	def read(self, fn):
		ret = self.__roint.read_RSM(self._base, fn)
		return(ret)
		
	def Dump(self, prefix):
		self.__roint.Dump_RSM(self._base, prefix)
    
	def getTextureCount(self):
		return(self.__roint.getTextureCount_RSM(self._base))
	
	"""Retrieve the number of meshed in this object"""
	def getMeshCount(self):
		r = self.__roint.getMeshCount_RSM(self._base)
		# print r
		return(r)
		
	def getTexture(self, idx):
		return(self._getTexture(self._base, idx))
	
	def getMeshHeader(self, mesh_id):
		"""TODO"""
		return(None)

	def getMeshTransf(self, mesh_id):
		"""TODO"""
		return(None)
	
	def getMeshVecCount(self, mesh_id):
		return(self.__roint.getMeshVecCount_RSM(self._base, mesh_id))
    
	def getMeshTexCount(self, mesh_id):
		return(self.__roint.getMeshTexCount_RSM(self._base, mesh_id))

	def getMeshTexvCount(self, mesh_id):
		return(self.__roint.getMeshTexvCount_RSM(self._base, mesh_id))
  
	"""Retrieve the number of surfaces in a given mesh"""
	def getMeshSurfCount(self, mesh_id):
		r = self.__roint.getMeshSurfCount_RSM(self._base, mesh_id)
		return(r)
  
	"""Return a given surface in a given mesh"""  
	def getMeshSurf(self, mesh_id, surf_id):
		return(self._getMeshSurf(self._base, mesh_id, surf_id))
    
	def getMeshVec(self, mesh_id, vec_id):
		return(self._getMeshVec(self._base, mesh_id, vec_id))
    
	def getMeshTexv(self, mesh_id, texv_id):
		return(self._getMeshTexv(self._base, mesh_id, texv_id))

# This is the test
if __name__ == "__main__":
  ro.printVersion();
  rsm = RSM()
  print("")
  print(rsm.read("test.rsm"))
  print("Texture count: %d" % rsm.getTextureCount())
  r = range(rsm.getTextureCount())
  for i in r:
    print(rsm.getTexture(i))
  print("Mesh count: %d" % rsm.getMeshCount())
  surfcount = rsm.getMeshSurfCount(0)
  print("Surface count: %d" % surfcount)
  for i in range(surfcount):
    print("-- %d --" % i)
    rsm.getMeshSurf(0,i)[0].Dump()
    
  veccount = rsm.getMeshVecCount(0)
  print("Vertices count: %d" % veccount)
  for i in range(veccount):
    print("-- %d --" % i)
    rsm.getMeshVec(0,i)[0].Dump()
    
  
  #rsm.Dump("")
  del(rsm)
