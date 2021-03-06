#!/usr/bin/env python
#
############################################################################
#
# MODULE:       v.delaunay2d
# AUTHOR(S):    Soeren Gebbert
# PURPOSE:      Delaunay 2d triangulation using the vtkDelaunay2D class.
#               The created vtk triangles will be converted into the grass
#               vector feature face.
#
# COPYRIGHT:    (C) 2009 Soeren Gebbert
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#%Module
#% description: Delaunay 2d triangulation using the vtkDelaunay2D class. The created triangles will be converted into the grass vector feature face.
#% keywords: vector
#% keywords: delaunay
#%End
#%Flag
#% key: t
#% description: Build vector topology
#%End
#%Flag
#% key: s
#% description: Show the input and output map in a vtk window after computation
#%End
#%Flag
#% key: w
#% description: Write the triangulated map as vtk xml file to the current working directoy named <output>.vtk
#%End
#%Option
#% key: input
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name of input vector map
#% gisprompt: old,vect,vector
#%End
#%Option
#% key: output
#% type: string
#% required: yes
#% multiple: no
#% key_desc: name
#% description: Name for output vector map
#% gisprompt: new,vect,vector
#%End
#%Option
#% key: alpha
#% type: double
#% required: no
#% multiple: no
#% key_desc: alpha
#% answer: 0
#% description: Specify alpha (or distance) value to control output of this filter. For a non-zero alpha value, only edges or triangles contained within a sphere centered at mesh vertices will be output. Otherwise, only triangles will be output.
#%End
#%Option
#% key: tolerance
#% type: double
#% required: no
#% multiple: no
#% key_desc: tolerance
#% answer: 0
#% description: Specify a tolerance to control discarding of closely spaced points. This tolerance is specified as a fraction of the diagonal length of the bounding box of the points.
#%End

#include the grass, VTK and vtkGRASSBridge Python libraries

from vtk import *
from libvtkGRASSBridgeIOPython import *
from libvtkGRASSBridgeCommonPython import *
from vtkGRASSBridgeModuleBase import *
import grass.script as grass


def main():
    input = options['input']
    output = options['output']
    alpha = options['alpha']
    tolerance = options['tolerance']
    build_topo = int(flags['t'])
    write_vtk = int(flags['w'])
    show = int(flags['s'])

    # Now build the pipeline
    # read the vector map without creating topology
    reader = vtkGRASSVectorPolyDataReader() 
    reader.SetVectorName(input)

    # start the delaunay triangulation
    delaunay = vtkDelaunay2D()
    delaunay.SetInputConnection(reader.GetOutputPort())
    delaunay.SetAlpha(float(alpha))
    delaunay.SetTolerance(float(tolerance))

    # Generate the output
    generateVectorOutput(build_topo, output, delaunay, write_vtk, show)

if __name__ == "__main__":
    # Initiate GRASS
    init = vtkGRASSInit()
    init.Init("v.delaunay2d")
    init.ExitOnErrorOn()
    
    options, flags = grass.parser()
    main()
