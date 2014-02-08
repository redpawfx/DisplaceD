cd /huge/develop/mll/projects/displacementD/linux
export MAYA_LOCATION=/usr/aw/maya8.0-x64
export LD_LIBRARY_PATH=$MAYA_LOCATION/lib

/usr/local/gcc402/bin/gcc402 -fPIC -c -I. -I.. -I/usr/aw/maya8.0-x64/include -m64 -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM -Wno-deprecated -fno-gnu-keywords ../DisplaceDeformer.cpp ../DisplaceDhandle.cpp ../PluginMain.cpp
/usr/local/gcc402/bin/gcc402 -shared -m64  -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic -o /huge/develop/mll/projects/displacementD/outputs/linux64/8.0/DisplaceD.so DisplaceDeformer.o DisplaceDhandle.o PluginMain.o -L/usr/aw/maya8.0-x64/lib -lFoundation -lOpenMaya -lOpenMayaFX -lOpenMayaUI -lImage -lOpenMayaAnim -lOpenMayaRender -lGLU
