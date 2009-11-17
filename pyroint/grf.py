# Copyright (C) 2008 Open-Ragnarok Project
# http://www.open-ragnarok.org
# $Id$

from ctypes import *
from _wrapper import Wrapper
import ro

class GRF(Wrapper):
	def __init__(self):
		self._base = self.Int().GRF_new();
		
		self.addFunction('open', 'GRF_open');
		self.addFunction('close', 'GRF_close');
		self.addFunction('isOpen', 'GRF_isOpen');
		self.addFunction('getCount', 'GRF_getCount', c_uint);
		self.addFunction('getFilename', 'GRF_getFilename', c_char_p);
		self.addFunction('del', 'GRF_del');
