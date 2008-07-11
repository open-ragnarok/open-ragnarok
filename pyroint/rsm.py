# $id$
from ctypes import *

roint = cdll.roint

functions = {
  'Version': '?Version@RO@@YAGXZ',
  'printVersion': '?printVersion@RO@@YAXXZ',
  '': ''
};

def Version():
  retval = roint[functions['Version']]()
  return(retval)

def printVersion():
  roint[functions['printVersion']]()

Version = roint[functions['Version']]
printVersion = roint[functions['printVersion']]


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
  _fields = [
    ("c", RSM_VecCoord),
    ("f", c_float * 3)
  ]
  
class RSM_Mesh_Header(Structure):
  _pack = 1
  _fields_ = [
    ("name",	c_char_p), # char[40]
    ("unk1",	c_int),
    ("parent",	c_char_p), # char[40]
    ("unk2",	c_float)
  ]

class RSM_Mesh_Transf(Structure):
  _pack = 1
  _fields = [
    ("mat33", c_float * 9),
    ("translate1", RSM_Vec),
    ("translate2", RSM_Vec), 
    ("rot_angle", c_float),
    ("rot_vector", RSM_Vec),
    ("scale", RSM_Vec)
  ]



class RSM:
  def __init__(self):
    self._base = roint.new_RSM() # store C++ superclass obj
    self._getTexture = roint.getTexture_RSM
    self._getTexture.restype = c_char_p
  
  def __del__(self):
    roint.del_RSM(self._base)
 
  def read(self, fn):
    ret = roint.read_RSM(self._base, fn)
    return(ret)
  
  def Dump(self, prefix):
    roint.Dump_RSM(self._base, prefix)
    
  def getTextureCount(self):
    return(roint.getTextureCount_RSM(self._base))
    
  def getMeshCount(self):
    return(roint.getMeshCount_RSM(self._base))
    
  def getTexture(self, idx):
    return(self._getTexture(self._base, idx))
  

# This is the test
if __name__ == "__main__":
  printVersion();
  rsm = RSM()
  print ""
  print rsm.read("../test1.rsm")
  print("Texture count: %d")
  print rsm.getTextureCount()
  print rsm.getTexture(0)
  #rsm.Dump("")
  del(rsm)
