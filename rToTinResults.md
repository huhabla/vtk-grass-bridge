# VTK GRASS example results of r.to.tin #

This page shows some results of the vtk-grass-bridge module r.to.tin, which uses grass as database unit and vtk as data processing unit.
All data is read directly from the grass raster database and written directly into the grass vector database.


# Module r.to.tin #

The [r.to.tin.py](http://code.google.com/p/vtk-grass-bridge/source/browse/trunk/Examples/r.to.tin.py) module converts a raster height map into a triangulated irregular network TIN vector map using the VTK class vtkGreedyTerrainDecimation.

Results using r.to.tin.py within the GRASS North Carolina sample dataset:

## Using 1000 triangles ##

Approximation of a 225000 raster cell height map with 1000 triangles:
```
r.to.tin.py input=elev_ned_30m output=elev_ned_30m_tin1000 trinum=1000 -wt 
```

Resulting vector map visualized with Paraview:

![http://vtk-grass-bridge.googlecode.com/files/elev_ned_30m_tin1000.png](http://vtk-grass-bridge.googlecode.com/files/elev_ned_30m_tin1000.png)

## Using 10000 triangles ##

Approximation of a 225000 raster cell height map with 10000 triangles:
```
r.to.tin.py input=elev_ned_30m output=elev_ned_30m_tin10000 trinum=10000 -wt 
```

Resulting vector map visualized with Paraview:

![http://vtk-grass-bridge.googlecode.com/files/elev_ned_30m_tin10000.png](http://vtk-grass-bridge.googlecode.com/files/elev_ned_30m_tin10000.png)


## Using 50000 triangles ##

Approximation of a 225000 raster cell height map with 50000 triangles:
```
r.to.tin.py input=elev_ned_30m output=elev_ned_30m_tin50000 trinum=50000 -wt 
```

Resulting vector map visualized with Paraview:

![http://vtk-grass-bridge.googlecode.com/files/elev_ned_30m_tin50000.png](http://vtk-grass-bridge.googlecode.com/files/elev_ned_30m_tin50000.png)
&lt;wiki:gadget url="http://www.ohloh.net/accounts/60588/widgets/account\_tiny.xml" height="40" border="0"/&gt;