# $Id$
from ctypes import *
from ro import *

class PAL_Color(Structure):
  """Palette color"""
  _pack_ = 1
  _fields_ = [
    ("r", c_ubyte),
    ("g", c_ubyte),
    ("b", c_ubyte),
    ("reserved", c_ubyte)
  ]

class PAL:
  """Palette class"""

  def __init__(self, pal=None):
    roint = cdll.roint

    self._new = roint.PAL_new
    self._new.argtypes = []
    self._new.restype = c_void_p

    self._del = roint.PAL_del
    self._del.argtypes = [c_void_p]
    self._del.restype = None

    self._copy = roint.PAL_copy
    self._copy.argtypes = [c_void_p, c_void_p]
    self._copy.restype = None

    self._read = roint.PAL_read
    self._read.argtypes = [c_void_p, c_char_p]
    self._read.restype = c_ubyte

    self._isValid = roint.PAL_isValid
    self._isValid.argtypes = [c_void_p]
    self._isValid.restype = c_ubyte

    self._getColor = roint.PAL_getColor
    self._getColor.argtypes = [c_void_p, c_ubyte]
    self._getColor.restype = POINTER(PAL_Color)

    from types import IntType
    if type(pal) == IntType:
      self._base = pal # address
    else:
      self._base = self._new()
      if pal != None:
        assert(isinstance(pal, PAL))
        self._copy(self._base, pal._base)

  def __del__(self):
    self._del(self._base)

  def read(self, fn):
    """Reads a palette from file"""
    return(bool(self._read(self._base, fn)))

  def isValid(self):
    """Checks if the palette is valid"""
    return(bool(self._isValid(self._base)))

  def getColor(self, idx):
    """Returns a palette color"""
    from copy import copy
    return(copy(self._getColor(self._base, idx)[0]))

if __name__ == "__main__":
  # test - read a palette
  pal = PAL()
  print "Read:", pal.read("../test.pal")
  print "Valid:", pal.isValid()
  for i in range(256):
    color = pal.getColor(i)
    print "Color #%d:" % i, color.r, color.g, color.b, color.reserved
  # test - can't change color inside PAL
  color = pal.getColor(0)
  r = color.r
  color.r = 255 - r
  color = pal.getColor(0)
  assert(r == color.r);
  # test - copy of a palette
  pal2 = PAL(pal)
  assert(pal.isValid() == pal2.isValid());
  for i in range(256):
    color = pal.getColor(i)
    color2 = pal2.getColor(i)
    assert(color.r == color2.r)
    assert(color.g == color2.g)
    assert(color.b == color2.b)
    assert(color.reserved == color2.reserved)
  del(pal2)
  del(pal)
