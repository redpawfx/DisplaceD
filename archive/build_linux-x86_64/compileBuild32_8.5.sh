cd /huge/develop/mll/projects/displacementD/linux
export version=8.5
export MAYA_LOCATION=/usr/autodesk/maya$version
export LD_LIBRARY_PATH=$MAYA_LOCATION/lib

/usr/local/gcc402/bin/gcc -c -I. -I.. -I$MAYA_LOCATION/include -I/usr/X11R6/include -m32 -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM -mcpu=pentium4 -Wno-deprecated -fno-gnu-keywords ../DisplaceDeformer.cpp ../DisplaceDhandle.cpp ../PluginMain.cpp
/usr/local/gcc402/bin/gcc -shared -m32 -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM  -mcpu=pentium4  -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic -o /huge/develop/mll/projects/displacementD/outputs/linux32/$version/DisplaceD.so DisplaceDeformer.o DisplaceDhandle.o PluginMain.o -L$MAYA_LOCATION/lib -lFoundation -lOpenMaya -lOpenMayaFX -lOpenMayaUI -lImage -lOpenMayaAnim -lOpenMayaRender -lGLU
