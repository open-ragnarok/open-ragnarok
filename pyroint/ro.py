# $id$
from ctypes import *

roint = cdll.roint

functions = {
  'Version': '?Version@RO@@YAGXZ',
  'printVersion': '?printVersion@RO@@YAXXZ',
  '': ''
};

Version = roint[functions['Version']]
printVersion = roint[functions['printVersion']]

#def euc2utf8(euc):
#  return(roint.euc2utf8(euc))
#  
#def utf82euc(utf):
#  return(roint.euc2utf8(utf))