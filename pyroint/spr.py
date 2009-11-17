# $Id$
from ctypes import *
from ro import *

roint = cdll.roint

class SPR_Pal(Structure):
  _pack_ = 1
  _fields_ = [
    ("r", c_char),
    ("g", c_char),
    ("b", c_char),
    ("a", c_char)
  ]
  
  def dump(self):
    print("%d %d %d %d" % (ord(self.r), ord(self.g), ord(self.b), ord(self.a)))
  
class SPR_Image(Structure):
  _pack_ = 1;
  _fields_ = [
    ("w", c_short),
    ("h", c_short),
    ("data", POINTER(c_char))
  ]
  
class SPR:
  def __init__(self):
    self._base = roint.new_SPR() # store C++ superclass obj
    self._getPal = roint.getPal_SPR
    self._getPal.restype = POINTER(SPR_Pal)

    self._getFrame = roint.getFrame_SPR
    self._getFrame.restype = POINTER(SPR_Image)
    
    print self._getPal
    print self._getFrame
  
  def __del__(self):
    roint.del_SPR(self._base)
    
  def read(self, fn):
    roint.read_SPR(self._base, fn)
    
  def getImgCount(self):
    return(roint.getImgCount_SPR(self._base))
  
  def saveBMP(self, idx, fn):
    return(roint.saveBMP_SPR(self._base, idx, fn))
    
  def getPal(self, idx):
    return(self._getPal(self._base, idx))
    
  def getFrame(self, idx):
    return(self._getFrame(self._base, idx))    
  
if __name__ == "__main__":
  print("Version:")
  printVersion()
  print("\n")
  spr = SPR()
  spr.read("../test.spr")
  print("We have %d images" % spr.getImgCount())
  # spr.saveBMP(1, "test-1.bmp")
  for i in range(spr.getImgCount()):
    x = spr.getFrame(i)
    print ("Frame sizes: %d x %d" % (x[0].w, x[0].h))
    
  del(spr)
  
