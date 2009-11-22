# $Id$
from ctypes import *
from ro import *
from pal import *
from copy import deepcopy


__all__ = [
  "SPR_Color",
  "SPR_Image",
  "SPR"
  ]


class SPR_Color(Structure):
  """RGBA color."""
  _pack_ = 1
  _fields_ = [
    ("a", c_ubyte),
    ("b", c_ubyte),
    ("g", c_ubyte),
    ("r", c_ubyte)
  ]
  
  def __ne__(self, other):
    """Inequality operator."""
    return not (self == other)

  def __eq__(self, other):
    """Equality operator."""
    return(isinstance(other, SPR_Color) and
           self.a == other.a and
           self.b == other.b and
           self.g == other.g and
           self.r == other.r)


class SPR_Image_Data(Union):
  """SPR image data."""
  _pack_ = 1
  _fields_ = [
    ("pal", POINTER(c_ubyte)),
    ("rgba", POINTER(SPR_Color))
  ]


class SPR_Image(Structure):
  """SPR image."""
  _pack_ = 1
  _anonymous_ = ["data"]
  _fields_ = [
    ("width", c_ushort),
    ("height", c_ushort),
    ("data", SPR_Image_Data),
    ("type", c_ubyte), # SPR.PalType/SPR.RgbaType
  ]
  
  def __ne__(self, other):
    """Inequality operator."""
    return not (self == other)

  def __eq__(self, other):
    """Equality operator."""
    return (isinstance(other, SPR_Image) and
            self.width == other.width and
            self.height == other.height and
            self.getData() == other.getData())

  def __deepcopy__(self, memo):
    """Deepcopy operator."""
    image = type(self)()
    image.width = self.width
    image.height = self.height
    image.type = self.type
    size = image.width * image.height
    if image.type == SPR.PalType:
      data = (c_ubyte * size)()
      data[:] = self.pal[:size]
      image.pal = data
    elif image.type == SPR.RgbaType:
      data = (SPR_Color * size)()
      data[:] = self.pal[:size]
      image.rgba = data
    else:
      assert False, "unsupported type %d" % image.type
    return(image)

  def getData(self):
    """Gets the data array."""
    size = self.width * self.height
    if self.type == SPR.PalType:
      return self.pal[:size]
    elif self.type == SPR.RgbaType:
      return self.rgba[:size]
    else:
      assert False, "unsupported type %d" % image.type


class SPR:
  """Sprite image class.  Contains images and maybe a palette."""

  PalType = 0
  RgbaType = 1

  Color = SPR_Color

  Image = SPR_Image

  def __init__(self, spr=None):
    """Constructor."""
    roint = cdll.roint

    self._new = roint.SPR_new
    self._new.argtypes = []
    self._new.restype = c_void_p

    self._del = roint.SPR_del
    self._del.argtypes = [c_void_p]
    self._del.restype = None

    self._copy = roint.SPR_copy
    self._copy.argtypes = [c_void_p, c_void_p]
    self._copy.restype = None

    self._read = roint.SPR_read
    self._read.argtypes = [c_void_p, c_char_p]
    self._read.restype = c_ubyte

    self._isValid = roint.SPR_isValid
    self._isValid.argtypes = [c_void_p]
    self._isValid.restype = c_ubyte

    self._getImageCount = roint.SPR_getImageCount
    self._getImageCount.argtypes = [c_void_p]
    self._getImageCount.restype = c_uint

    self._getImageCountByType = roint.SPR_getImageCountByType
    self._getImageCountByType.argtypes = [c_void_p, c_ubyte]
    self._getImageCountByType.restype = c_uint

    self._getImage = roint.SPR_getImage
    self._getImage.argtypes = [c_void_p, c_uint]
    self._getImage.restype = POINTER(SPR_Image)

    self._getImageByType = roint.SPR_getImageByType
    self._getImageByType.argtypes = [c_void_p, c_uint, c_ubyte]
    self._getImageByType.restype = POINTER(SPR_Image)

    self._getIndex = roint.SPR_getIndex
    self._getIndex.argtypes = [c_void_p, c_uint, c_ubyte]
    self._getIndex.restype = c_uint

    self._getPal = roint.SPR_getPal
    self._getPal.argtypes = [c_void_p]
    self._getPal.restype = c_void_p

    self._base = self._new()
    from types import IntType
    if isinstance(spr, IntType):
      self._copy(self._base, spr) # copy from address
    elif spr is not None:
      assert isinstance(spr, SPR), "expected a SPR instance, got %s" % type(spr)
      self._copy(self._base, spr._base) # copy from SPR

  def __del__(self):
    """Destructor."""
    self._del(self._base)

  def __ne__(self, other):
    """Inequality operator."""
    return not (self == other)

  def __eq__(self, other):
    """Equality operator."""
    if (isinstance(other, SPR) and
        self.isValid() == other.isValid()):
      for imgtype in [SPR.PalType, SPR.RgbaType]:
        count = self.getImageCount(imgtype)
        images = self._getImageByType(self._base, 0, imgtype)
        images2 = other._getImageByType(other._base, 0, imgtype)
        if (other.getImageCount(imgtype) == count and images[:count] == images2[:count]):
          continue
        return False # image array doesn't match
      return self.getPal() == other.getPal()
    return False

  def read(self, fn):
    """Reads a SPR from file."""
    return(bool(self._read(self._base, fn)))

  def isValid(self):
    """Checks if the SPR is valid."""
    return(bool(self._isValid(self._base)))

  def getImageCount(self, imgtype=None):
    """Returns the number of images."""
    if imgtype is None:
      return(self._getImageCount(self._base))
    else:
      return(self._getImageCountByType(self._base, imgtype))

  def getImage(self, idx, imgtype=None):
    """Returns an image or None."""
    image = None
    if imgtype is None:
      image = self._getImage(self._base, idx)
    else:
      image = self._getImageByType(self._base, idx, imgtype)
    if image is not None:
      image = deepcopy(image[0])
    return(image)

  def getImages(self, imgtype=None):
    """Returns an array of images."""
    count = self.getImageCount(imgtype)
    images = (SPR_Image * count)()
    for i in range(count):
      images[i] = self.getImage(i, imgtype)
    return images

  def getIndex(self, idx, imgtype):
    """Returns the index of the image."""
    return self._getIndex(self._base, idx, imgtype)

  def getPal(self):
    """Returns the palette.  Might not be valid."""
    return PAL(self._getPal(self._base))

if __name__ == "__main__":
  spr = SPR()
  if 1:# test - read from file
    print "Read:", spr.read("../test.spr")
    print "Valid:", spr.isValid()
    print "Images:", spr.getImageCount()
    print "Pal images:", spr.getImageCount(spr.PalType)
    print "Rgba images:", spr.getImageCount(spr.RgbaType)
  if 2:# test - match image array and individual images
    for imgtype in [None, spr.PalType, spr.RgbaType]:
      count = spr.getImageCount(imgtype)
      images = spr.getImages(imgtype)
      for i in range(count):
        assert spr.getImage(i, imgtype) == images[i]
  if 3:# test - match images of global indexes and type indexes
    assert spr.getImageCount() == spr.getImageCount(spr.PalType) + spr.getImageCount(spr.RgbaType)
    for imgtype in [spr.PalType, spr.RgbaType]:
      for i in range(spr.getImageCount(imgtype)):
        assert spr.getImage(spr.getIndex(i, imgtype)) == spr.getImage(i, imgtype)      
  if 4:# test - can't change image inside SPR
    for imgtype in [None, spr.PalType, spr.RgbaType]:
      if spr.getImageCount(imgtype) == 0:
        continue
      images = spr.getImages(imgtype)
      width = images[0].width
      images[0].width = width + 1
      images = spr.getImages(imgtype)
      assert images[0].width == width
      image = spr.getImage(0, imgtype)
      width = image.width
      image.width = width + 1
      image = spr.getImage(0, imgtype)
      assert image.width == width
  if 5:# test - copy from address
    spr2 = SPR(spr._base)
    assert spr._base != spr2._base
    assert spr == spr2
    del spr2
  if 6:# test - copy from SPR
    spr2 = SPR(spr)
    assert spr._base != spr2._base
    assert spr == spr2
    del spr2
  if 7:# test - print images
    images = spr.getImages()
    for i in range(spr.getImageCount()):
      image = images[i]
      print "Image #%d:" % i, image.width, image.height, image.type, image.getData()
  del spr
