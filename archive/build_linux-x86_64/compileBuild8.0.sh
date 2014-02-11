cd /huge/develop/mll/projects/displacementD/linux
export MAYA_LOCATION=/usr/aw/maya8.0
export LD_LIBRARY_PATH=$MAYA_LOCATION/lib

/usr/local/gcc402/bin/gcc -c -I. -I.. -I/usr/aw/maya8.0/include -I/usr/X11R6/include -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM -mcpu=pentium4 -Wno-deprecated -fno-gnu-keywords ../DisplaceDeformer.cpp ../DisplaceDhandle.cpp ../PluginMain.cpp
/usr/local/gcc402/bin/gcc -shared  -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM  -mcpu=pentium4  -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic -o /huge/develop/mll/projects/displacementD/outputs/linux32/8.0/DisplaceD.so DisplaceDeformer.o DisplaceDhandle.o PluginMain.o -L/usr/aw/maya8.0/lib -lFoundation -lOpenMaya -lOpenMayaFX -lOpenMayaUI -lImage -lOpenMayaAnim -lOpenMayaRender -lGLU
