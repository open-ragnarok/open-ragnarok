# Copyright (C) 2008-2009 Open-Ragnarok Project
# http://www.open-ragnarok.org
# $Id$

# These have been tested using Python 2.6.

"""
A package for parsing, handling, and generating Ragnarok Online files.

You should put all these files inside a folder called RO and use 'import ro' or 'from ro import *'
"""

__version__ = "0.1"

__all__ = [
  "spr",
  "ro",
  "rsm",
  "act",
  "grf"
  ]
  
import spr
import ro
import rsm
import act
import grf

#SPR = spr.SPR
#RSM = rsm.RSM
  
#from spr import *
#from ro import *
#from rsm import *

