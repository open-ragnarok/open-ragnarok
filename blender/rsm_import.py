#!BPY
"""
Name: 'RSM Import (.rsm)...'
Blender: 244
Group: 'Import'
Tooltip: 'Import from Ragnarok RSM file format (.rsm)'
"""

__author__= ['Luis Sergio S. Moura Jr.']
__url__ = ("http://www.icetempest.com/blog")
__version__= '0.1'
__bpydoc__= '''\
No documentation yet
'''

"""$Id$"""
# ***** BEGIN GPL LICENSE BLOCK *****
#
# Script copyright (C) Bob Holcomb 
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# ***** END GPL LICENCE BLOCK *****


#import Blender
#from Blender import Texture,Image,Material
#from Blender.Window import EditMode, GetCursorPos, GetViewQuat

# import needed stuff
import os.path
import math
from math import *
import struct
import Blender
from Blender import Mathutils
from Blender.Mathutils import *

import bpy
import BPyMessages
import BPyAddMesh
from Blender import Texture,Image,Material
from Blender.Window import EditMode, GetCursorPos, GetViewQuat

# RO stuff
from pyroint.rsm import *
from pyroint.strings import *

import locale

print("Locale %s" % locale.setlocale(locale.LC_ALL, ''))
locale.setlocale(locale.LC_ALL, 'C') 

def getRSM(rsm):
  verts = []
  faces = []
  rsm_faces = []
  
  all_texv = []
  texv = []
  
  veccount = rsm.getMeshVecCount(0)
  facecount = rsm.getMeshSurfCount(0)
  texvcount = rsm.getMeshTexvCount(0)
  for i in range(veccount):
    v = rsm.getMeshVec(0,i)[0]
    verts.append([v.f[0], v.f[1], v.f[2]])
    
  for i in range(texvcount):
    v = rsm.getMeshTexv(0, i)[0]
    all_texv.append([v.f[0], v.f[1], 1 - v.f[2]])

  for i in range(facecount):
    texr = []
    f = rsm.getMeshSurf(0, i)[0]
    faces.append([f.sv[0], f.sv[1], f.sv[2]])
    texr.append([all_texv[f.tv[0]][1], all_texv[f.tv[0]][2]])
    texr.append([all_texv[f.tv[1]][1], all_texv[f.tv[1]][2]])
    texr.append([all_texv[f.tv[2]][1], all_texv[f.tv[2]][2]])

    texv.append(texr)
    
    rsm_faces.append(f)
  
    
  
  return(verts, texv, faces, rsm_faces)
  
def createImage(fn):
  try:
    img = Image.Load(fn);
  except:
    return(None)
    
  return(img)
  
  
def createMat(texname, path = ""):
  filename = "texture/" + texname.replace('\\','/')
  
  ffn = "%s%s" % (path, filename)
  
  #dn = filename.decode('utf-8')
  
  #print filename
  #print dn
  
  # texture
  texture = Texture.New(texname)
  texture.setType('Image')
  # texture.uvlayer = texname
  
  img = createImage(ffn)
  if (img == None):
    print "Can't find file %s" % ffn
    nfn = "%s%s" % (path, getFile(filename))
    img = createImage(nfn)
  if (img == None):
    print "Can't find file %s. You're on your own..." % nfn
    img = Image.New(getFile(ffn), 10, 10, 1)

  #try:
  #  img = Image.Load(ffn);
  #  #img = Image.Load(dn);
  #except:
  #  print ("Can't find texture %s. You'll have to assign it on your own" % ffn)
  #  img = Image.New(ffn, 10, 10, 1)

  texture.image = img

  mat = Material.New(texname)
  mat.setTexture(3, texture, Blender.Texture.TexCo.UV)
  
  # TODO: Optimize this
  for m in mat.getTextures():
    if (m != None):
      m.uvlayer = texname
  
  return(mat)
  
def removeDup(vec):
  """Create a list without duplicate objects from vec"""
  ret = []
  for i in vec:
    try:
      ret.index[i]
    except:
      ret.append(i)
      
  return(ret)
  
def getPath(path):
  npath = path.replace('\\','/')
  plist = npath.split('/')
  plist.pop()
  ret = ""
  for c in plist:
    ret = "%s%s/" % (ret, c)
  
  return(ret)
  
def getFile(path):
  npath = path.replace('\\','/')
  plist = npath.split('/')
  ret = plist.pop()
  return(ret)
  
  
def fileCallback(filename):
  if(EditMode()):
    EditMode(0) # Exit edit mode


  # reference: http://www.blender.org/documentation/246PythonDoc/Texture-module.html
  textures = {}
  
  #rsm_fn = "x:/test1.rsm"
  rsm_fn = filename
  rsm = RSM()
  if (rsm.read(rsm_fn) == 0):
    Blender.Draw.PupMenu("Error reading RSM file.", 2)
    return
  
  
  Mesh_ID = 0
  
  name = 'RSM_Imported'  
  verts, texv, faces, rsm_faces = getRSM(rsm)
  
  # create the object
  scn = bpy.data.scenes.active
  cursor = GetCursorPos()

  me = bpy.data.meshes.new(name)
  me.verts.extend(verts)
  me.faces.extend(faces)
  
  # assign mesh to object and position it
  ob_act = scn.objects.new(me, name)
  scn.objects.active = ob_act
  ob_act.loc = cursor
  me.calcNormals()  
  
  
  # These are vertex groups for use with our UV later
  vertgroups = []

  # Generating one UV layer per texture
  curpath = getPath(filename)
  print filename
  print curpath
  texnames_utf = []
  for i in range(rsm.getMeshTexCount(Mesh_ID)):
      name_euc = rsm.getTexture(i)
      name_utf = euc2utf8(name_euc)
      #name_utf = name_euc.encode('utf-8')

      me.addUVLayer(name_utf)
      me.addVertGroup(name_utf)
      texnames_utf.append(name_utf)
      vertgroups.append([])

  # ===== MATERIALS =====
  # Assign vertexes to group
  mat = []
  # this holds the colbits for the mesh
  val = 0
  for i in range(rsm.getMeshTexCount(Mesh_ID)):
    name = texnames_utf[i]
  
    nodup = removeDup(vertgroups[i])
    me.assignVertsToGroup(name, nodup, 1.0, Blender.Mesh.AssignModes.REPLACE)
    
    # Create materal for that group
    x = createMat(name, curpath)
    mat.append(x)
    val += 1 << i
    
  # assign materials to the object
  me.materials = mat
  ob_act.setMaterials(mat)
  ob_act.colbits = val
  
  # set uvlayers
  #try:
  #  for i in range(rsm.getMeshTexCount(Mesh_ID)):
  #    me.materials[i].uvlayer = texnames_utf[i]
  #except:
  #  print("ERROR: Can't set uvlayer names...Why? WHY!?")
  
  # ===== /MATERIALS =====
  
  

  
  i = 0
  for f in me.faces:
    curtexname = texnames_utf[rsm_faces[i].texid]
    me.activeUVLayer = curtexname
    group_id = texnames_utf.index(curtexname)
    # Creating group of vertices
    rface = rsm_faces[i]
    vertgroups[group_id].append(rface.sv[0])
    vertgroups[group_id].append(rface.sv[1])
    vertgroups[group_id].append(rface.sv[2])
    
    # Generating UV coordinates for each face  
    k = 0
    for x in f.uv:
      x[0] = texv[i][k][0]
      x[1] = texv[i][k][1]
      k = k + 1
    i = i + 1
    
    # Assigning face to material
    f.mat = rface.texid
    
   
  # print vertgroups
  # BPyAddMesh.add_mesh_simple(name, verts, [], faces)


Blender.Window.FileSelector(fileCallback, 'Import')
# main()


'''
  GRF_prefix = "d:/sergio/openro/data/data"
  texture_dir = GRF_prefix + "/texture"

  texture = "chdesk-side1"

  verts = []
  faces = []

  # Create CUBE
  verts.append([-1, -1, -1])
  verts.append([ 1, -1, -1])
  verts.append([-1,  1, -1])
  verts.append([ 1,  1, -1])
  verts.append([-1, -1,  1])
  verts.append([ 1, -1,  1])
  verts.append([-1,  1,  1])
  verts.append([ 1,  1,  1])

  # Create faces
  faces.append([4, 2, 0]) # x = -1
  faces.append([2, 4, 6])

  faces.append([1, 3, 5]) # x = 1
  faces.append([7, 5, 3])

  faces.append([0, 1, 4]) # y = -1
  faces.append([5, 4, 1])

  faces.append([6, 3, 2]) # y = 1
  faces.append([3, 6, 7])

  faces.append([2, 1, 0]) # z = -1
  faces.append([1, 2, 3])
  
  faces.append([4, 5, 6]) # z = 1
  faces.append([7, 6, 5])

  #	Adds the mesh to the scene
  name = 'RSM1'

  # BPyAddMesh.add_mesh_simple('RSM1', verts, [], faces)
  
  # texture
  textures[texture] = Texture.New(texture)
  textures[texture].setType('Image')
  img = Image.Load(texture_dir + "/" + texture + ".bmp")
  textures[texture].image = img

  mat = Material.New(name)
  mat.setTexture(0, textures[texture])

  # create the object
  scn = bpy.data.scenes.active
  cursor = GetCursorPos()

  me = bpy.data.meshes.new(name)
  me.verts.extend(verts)
  me.faces.extend(faces)
  me.materials.new(mat)
  # object location
  ob_act = scn.objects.new(me, name)
  scn.objects.active = ob_act
  ob_act.loc = cursor

  me.calcNormals()
'''
