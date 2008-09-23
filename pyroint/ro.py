# $Id$
from ctypes import *

_roint = None
def Int():
	global _roint
	if (_roint == None):
		_roint = cdll.roint
		
	return(_roint)

functions = {
  'Version': '?Version@RO@@YAGXZ',
  'printVersion': '?printVersion@RO@@YAXXZ',
  '': ''
};

Version = Int()[functions['Version']]
printVersion = Int()[functions['printVersion']]

#def euc2utf8(euc):
#  return(roint.euc2utf8(euc))
#  
#def utf82euc(utf):
#  return(roint.euc2utf8(utf))

if __name__ == "__main__":
	print("Using version %s" % Version());
