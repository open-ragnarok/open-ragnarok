# $Id$
from ctypes import *
from ro import *

roint = cdll.roint

"""Palette color"""
class PAL_Color(Structure):
  _pack_ = 1
  _fields_ = [
    ("r", c_ubyte),
    ("g", c_ubyte),
    ("b", c_ubyte),
    ("reserved", c_ubyte)
  ]

"""Palette class"""
class PAL:
  def __init__(self):
    self._base = roint.PAL_new()

    self._read = roint.PAL_read
    self._read.argtypes = c_void_p, c_char_p
    self._read.restype = c_ubyte

    self._isValid = roint.PAL_isValid
    self._isValid.argtypes = c_void_p
    self._isValid.restype = c_ubyte

    self._getColor = roint.PAL_getColor
    self._getColor.argtypes = c_void_p, c_ubyte
    self._getColor.restype = POINTER(PAL_Color)
  
  def __del__(self):
    roint.PAL_del(self._base)

  """Reads a palette from file"""
  def read(self, fn):
    return(bool(self._read(self._base, fn)))

  """Returns if the palette is valid"""
  def isValid(self):
    return(bool(self._isValid(self._base)))

  """Returns a palette color"""
  def getColor(self, idx):
    return(self._getColor(self._base, idx))

# TODO test
if __name__ == "__main__":
  
