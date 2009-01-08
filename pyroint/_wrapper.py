# Copyright (C) 2008 Open-Ragnarok Project
# http://www.open-ragnarok.org
# $Id$

import ro

class Wrapper:
	__roint = None
	_base = None
	_functions = {}

	class Callable:
		_func = None
		_base = None
		_name = None
		def __init__(self, func, base):
			self._func = func
			self._base = base
		def __call__(self, *args):
			#print("Calling as function '%s' with %d parameters " % (self._name, len(args)) , args);
			#print("Base: ", self._base);
			ret = None
			if (len(args) > 0):
				ret = self._func(self._base, *args);
			else:
				ret = self._func(self._base);
			return(ret)
	def __getattr__(self, name):
		ret = None
		#print("GETATTR: %s\n" % name);
		for k in self._functions.keys():
			if (k == name):
				ret = Wrapper.Callable(self._functions[k], self._base)
				ret._name = k
				return(ret)
				#return(self._functions[k])
		return(AttributeError())
	def __del__(self):
		"""
		Deletes the object. If there is a 'del' function, it's called passing the base as argument.
		"""
		for k in self._functions.keys():
			if (k == 'del'):
				self._functions['del'](self._base);
	def addFunction(self, name, funcname, restype = None):
		"""
		Register a callback function for use with this class as parameter.
		"""
		self._functions[name] = self.Int()[funcname];
		if (restype != None):
			self._functions[name].restype = restype;
	def Int(self):
		"""
		Grabs the interface dll
		"""
		if (self.__roint == None):
			self.__roint = ro.Int()
		return(self.__roint);
