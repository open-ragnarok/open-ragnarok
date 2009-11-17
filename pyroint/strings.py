# $Id$
from ctypes import *

roint = cdll.roint

#roint.euc2utf8.restype = c_char_p
#roint.utf82euc.restype = c_char_p

def euc2utf8(euc):
  ptr = roint.euc2utf8(euc)
  str = string_at(ptr)
  return(str)
  
def utf82euc(utf):
  ptr = roint.utf82euc(utf)
  str = string_at(ptr)
  return(str)
  