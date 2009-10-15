# Copyright (C) 2008 Open-Ragnarok Project
# http://www.open-ragnarok.org
# $Id$

from ctypes import *
import sys, ro, os

import rsm

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        if (os.path.exists('test.rsm') == False):
            print("Usage:\n%s <filename.rsm>" % sys.argv[0]);
            sys.exit(1);
        else:
            fn = 'test.rsm'
    else:
        fn = sys.argv[1]
        if (os.path.exists(fn) == False):
            print("Can't find file %s" % fn)
            sys.exit(1)

    f = rsm.RSM();
    print(f.read(fn))


    
    
