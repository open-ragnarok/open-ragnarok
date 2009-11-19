# $Id$
from ctypes import *
from ro import *


__all__ = [
  "PAL_Color",
  "PAL"
  ]


class PAL_Color(Structure):
  """Palette color"""
  _pack_ = 1
  _fields_ = [
    ("r", c_ubyte),
    ("g", c_ubyte),
    ("b", c_ubyte),
    ("reserved", c_ubyte)
  ]

  def __ne__(self, other):
    """Inequality operator."""
    return(not(self == other))

  def __eq__(self, other):
    """Equality operator."""
    return(isinstance(other, PAL_Color) and
           self.r == other.r and
           self.g == other.g and
           self.b == other.b and
           self.reserved == other.reserved)


class PAL:
  """Palette class.  Contains 256 colors."""

  Color = PAL_Color

  def __init__(self, pal=None):
    """Constructor."""
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

    self._base = self._new()
    from types import IntType
    if isinstance(pal, IntType):
      self._copy(self._base, pal) # copy from address
    elif pal is not None:
      assert isinstance(pal, PAL)
      self._copy(self._base, pal._base) # copy from PAL

  def __del__(self):
    """Destructor."""
    self._del(self._base)

  def __ne__(self, other):
    """Inequality operator."""
    return not (self == other)

  def __eq__(self, other):
    """Equality operator."""
    if not isinstance(other, PAL):
      return False
    if self.isValid() != other.isValid():
      return False
    colors = self._getColor(self._base, 0)
    colors2 = other._getColor(other._base, 0)
    if colors[:256] != colors2[:256]:
      return False
    return True

  def read(self, fn):
    """Reads a palette from file."""
    return bool(self._read(self._base, fn))

  def isValid(self):
    """Checks if the palette is valid."""
    return bool(self._isValid(self._base))

  def getColor(self, idx):
    """Returns a palette color."""
    from copy import deepcopy
    return deepcopy(self._getColor(self._base, idx)[0])

  def getColors(self):
    """Returns an array with all palette colors."""
    p = self._getColor(self._base, 0)
    colors = (PAL_Color * 256)()
    colors[:] = p[:256]
    return colors


if __name__ == "__main__":
  pal = PAL()
  if 1:# test - read a palette file
    print "Read:", pal.read("../test.pal")
    print "Valid:", pal.isValid()
  if 2:# test - match color array and individual colors
    colors = pal.getColors()
    for i in range(256):
      assert pal.getColor(i) == colors[i]
  if 3:# test - can't change a color inside PAL
    colors = pal.getColors()
    r = colors[0].r
    colors[0].r = 255 - r
    colors = pal.getColors()
    assert colors[0].r == r
    color = pal.getColor(0)
    r = color.r
    color.r = 255 - r
    color = pal.getColor(0)
    assert color.r == r
  if 4:# test - copy from address
    pal2 = PAL(pal._base)
    assert pal._base != pal2._base
    assert pal == pal2
    del pal2
  if 5:# test - copy from PAL
    pal2 = PAL(pal)
    assert pal._base != pal2._base
    assert pal == pal2
    del pal2
  if 6:# test - print colors
    colors = pal.getColors()
    for i in range(256):
      color = colors[i]
      print "Color #%d:" % i, color.r, color.g, color.b, color.reserved
  del pal
