# Copyright (C) 2008 Open-Ragnarok Project
# http://www.open-ragnarok.org
# $Id$

from ctypes import *
from _wrapper import Wrapper
import ro

class ACT(Wrapper):
	def __init__(self):
		self._base = self.Int().ACT_new() # store C++ superclass obj
		
		# Load functions
		self.addFunction('del', 'ACT_del')
		self.addFunction('dump', 'ACT_dump')
		self.addFunction('read', 'ACT_read')
