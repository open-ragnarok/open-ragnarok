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
