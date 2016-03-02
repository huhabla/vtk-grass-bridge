# HowTo compile the vtk-grass-bridge #

## Introduction ##

The vtk-grass-bridge depends on cmake for configuration and on several third party libraries. The two main libraries are **VTK** and **grass7**. Additionally gdal and proj4 libraries should be present.

The vtk-grass-bridge works with VTK versions 5.2, 5.4 and 5.6. Two optional features of the vtk-grass-bridge depend on VTK 5.9:
  * Temporal pipeline support
  * R-VTK interface

These optional featurs are in an experimental state and disabled by default.

The gdal and proj4 libraries should be available on your system via the package manager. These libraries are needed by VTK and by grass7 as well.

To use the vtk-grass-bridge in your project you need to:
  * Compile and install cmake
  * Compile and install VTK
  * Compile and install grass7
  * Compile and install vtk-grass-bridge
  * Set up some environment variables
  * Test the vtk-grass-bridge

## Compile/Install CMake ##

You will need cmake to configure the vtk-grass-bridge as well as VTK itself. Use your package manager for installation or download the latest version here:

http://www.cmake.org/cmake/resources/software.html

Extract, compile and install cmake. On unix this simple shell script
should do the job:
```
#!/bin/sh
wget http://www.cmake.org/files/v2.8/cmake-2.8.4.tar.gz
tar xzvf cmake-2.8.4.tar.gz 
cd cmake-2.8.4 
./configure --qt-gui && make && make install
```

## Compile GRASS GIS 7 ##

Download the latest grass7 svn version via svn from the osgeo server. You can use this script to do the job (pleas edit to your neededs):
```
#!/bin/sh
mkdir grass7.0
cd grass7.0
svn checkout https://svn.osgeo.org/grass/grass/trunk grass_trunk
cd grass_trunk
make distclean
export CFLAGS="-ggdb -Wall -Werror-implicit-function-declaration -fno-common -fopenmp"
export LDFLAGS="-lgomp"
./configure  --without-freetype --with-wxwidgets --with-sqlite --with-postgresql --with-cxx --with-python && make -j12
```

You don't need to install GRASS, the build directory contains a full functional grass installation.

## Compile/Install VTK ##

Use your package manager to install VTK. Assure that Python support is enabled. In case you want to use Java to code grass7 modules make sure that Java support is available too via package manager.

If no VTK package is available you need to compile VTK by yourself. Download the latests stable version here:
http://www.vtk.org/VTK/resources/software.html

Extract the sources in your source directory and create a new build directory. Use cmake-gui to configure VTK. Enable shared library, Python and Java support. After configuration compile and install VTK on your computer(this may take a while). The default installation path is `/usr/local/lib/vtk-5.6` in case of VTK 5.6.
```
#!/bin/sh
mkdir VTK_5.6
cd VTK_5.6
wget http://www.vtk.org/files/release/5.6/vtk-5.6.1.tar.gz
tar xzvf vtk-5.6.1.tar.gz
mkdir VTK_Build
cd VTK_Build
cmake-gui ../VTK
make && make install
```
Here a screenshot of a valid VTK configuration:
![http://vtk-grass-bridge.googlecode.com/files/vtk-cmake-gui.png](http://vtk-grass-bridge.googlecode.com/files/vtk-cmake-gui.png)

## Compile/Install vtk-grass-bridge ##

After the compilation/installation of grass7 and VTK you can configure and compile the vtk-grass-bridge.

Download the latest code via svn:
```
svn checkout http://vtk-grass-bridge.googlecode.com/svn/trunk/ vtk-grass-bridge
```
Create a new build directory:
```
mkdir vtk-grass-bridge_build_VTK5.6
```
Use cmake-gui to configure the vtk-grass-bridge.
```
cd vtk-grass-bridge_build_VTK5.6
cmake-gui ../vtk-grass-bridge
```

The paths to your VTK installation, to the grass7 library and include directory as well as the gdal and proj4 libraries must be set.
Have a look at this screenshot showing a valid configuration:

![http://vtk-grass-bridge.googlecode.com/files/vtk-grass-bridge-cmake-gui.png](http://vtk-grass-bridge.googlecode.com/files/vtk-grass-bridge-cmake-gui.png)

After configuration compile the sources on Unix with
```
make
```
and install them using
```
make install
```
The libraries are installed by default in `/usr/local/lib`.

## Configure your environment ##

To use the vtk-grass-bridge in your projects or to run the Python tests, examples and modules you need to assure that the grass7-, vtk-grass-bridge- and the VTK-libraries are located in your LD\_LIBRARY\_PATH. Additionally the vtk-grass-bridge libraries and the VTK Python package must be in your PYTHONPATH. Here an example for the bash:

```
export PYTHONPATH="/usr/local/lib/python2.6/site-packages/:/usr/local/lib"
export LD_LIBRARY_PATH="/usr/local/lib:/home/soeren/src/grass7.0/grass_trunk/dist.x86_64-unknown-linux-gnu/lib:/usr/local/lib/vtk-5.6"
```

## Java support ##

To use the vtk-grass-bridge in your Java programs the vtk-grass-bridge and VTK must be compiled with Java support. The automatically generated Java classes located in vtk-grass-bridge\_build\_VTK5.6/java/vtk must be compiled and a new vtk.jar including the VTK and vtk-grass-bridge classes must be created. The new Java archive must be present in your CLASSPATH environment variable.

This little shell script should help you. Make sure you are in the vtk-grass-bridge build directory:
```
#!/bin/sh
cd java/vtk
cp /usr/local/lib/vtk-5.6/java/vtk.jar ../
javac -classpath ../vtk.jar *.java
cd ..
jar xvf vtk.jar
jar cvf vtkGrass.jar vtk
rm -rf vtk/*
cd ..
make
```

Now put the vtkGrass.jar archive into your CLASSPATH variable. Additionally add the Java xawt library to your LD\_LIBRARY\_PATH:

```
export LD_LIBRARY_PATH="/usr/lib64/jvm/java-1.6.0-sun-1.6.0/jre/lib/amd64/xawt:$LD_LIBRARY_PATH"
```

## Test your installation ##

We will now test the vtk-grass-bridge installation. Make sure all paths (PYTHONPATH, LD\_LIBRARY\_PATH and CLASSPATH) are set correctly.

The vtk-grass-bridge examples, tests and modules must be run in the grass shell environment. Start grass7 with the spearfish6 LOCATION using MAPSET PERMANENT.

First we need to rebuild topology for all vector maps, otherwise vtk-grass-bridge and grass7 modules will have problems to open them in topology mode:

```
#!/bin/sh
# Rebuild topology for grass7 use
for i in `g.mlist vect` ; do 
  v.build $i; 
done
```

To test the correct installation of the vtk-grass-bridge with Python and Java support we will run a Cxx, Python and a Java module with similar functionality. The modules sample several raster maps using points of a vector map.

```
#!/bin/sh
# Make sure we are in the source folder of vtk-grass-bridge

# First we start the Cxx version of v.sample.rast
# which was installed in /usr/local/bin
v.sample.rast --o input=archsites raster=elevation.dem,soils output=sample_0
db.select "select * from sample_0" > /tmp/sample_0.txt

# Then we start the Python version of v.sample.rast
cd Modules/Python
python v.sample.rast.py --o input=archsites raster=elevation.dem,soils output=sample_1
db.select "select * from sample_1" > /tmp/sample_1.txt

# Compare the Cxx and Python version using diff
diff /tmp/sample_0.txt /tmp/sample_1.txt

# Now start the Java implementation and compare the results
cd ../Java
# Compile the java program first
javac -classpath $CLASSPATH v_sample_rast.java
java -classpath $CLASSPATH:. v_sample_rast --o input=archsites raster=elevation.dem,soils output=sample_2

db.select "select * from sample_2" > /tmp/sample_2.txt

# Compare the Python and Java results using diff
diff /tmp/sample_1.txt /tmp/sample_2.txt
```

The vtk-grass-bridge is ready to use when no error occurred and the diff command shows no difference between the text files.

## HowTo use the vtk-grass-bridge ##

To use the vtk-grass-bridge please study the [examples](http://code.google.com/p/vtk-grass-bridge/source/browse/trunk/#trunk%2FExamples), [tests](http://code.google.com/p/vtk-grass-bridge/source/browse/trunk/#trunk%2FVector%2FTesting%2FPython) and vtk-grass-bridge [modules](http://code.google.com/p/vtk-grass-bridge/source/browse/trunk/#trunk%2FModules%2FPython).

The vtk-grass-bridge is documented using doxygen. You can generate the documentation from the source directory:

```
#!/bin/sh
# Make sure you are in the vtk-grass-bridge directory
doxygen Doxyfile
# Switch into the documentation dir
cd ../vtkGRASSBridgeDoc/html/
# Call firefox to read the docs
firefox index.html
```