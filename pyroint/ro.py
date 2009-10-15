# Copyright (C) 2008-2009 Open-Ragnarok Project
# http://www.open-ragnarok.org
# $Id$

from ctypes import *

_roint = None


functions = {
  'Version': '?Version@RO@@YAGXZ',
  'printVersion': '?printVersion@RO@@YAXXZ',
  'setupLogs': '?SetupLogs@ro@@YAXXZ',
  '': ''
};

def Int():
	global _roint
	global functions
	if (_roint == None):
		_roint = cdll.roint
		#_roint[functions['setupLogs']]()
		
	return(_roint)

#Version = Int()[functions['Version']]
#printVersion = Int()[functions['printVersion']]
Version = Int()['Version']
printVersion = Int()['printVersion']

#def euc2utf8(euc):
#  return(roint.euc2utf8(euc))
#  
#def utf82euc(utf):
#  return(roint.euc2utf8(utf))

if __name__ == "__main__":
	print("Using version %s" % Version());
