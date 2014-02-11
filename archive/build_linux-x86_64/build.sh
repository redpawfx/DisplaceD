cd /huge/develop/mll/maya/8.0/displacementD/linux
export MAYA_LOCATION=/usr/aw/maya8.0
export LD_LIBRARY_PATH=$MAYA_LOCATION/lib

/usr/local/gcc334/bin/gcc -shared  -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM  -mcpu=pentium4  -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic -o /huge/maya_setups/hajime/8.0/linuxPlugin/DisplaceD.so DisplaceDeformer.o DisplaceDhandle.o PluginMain.o -L/usr/aw/maya8.0/lib -lFoundation -lOpenMaya -lOpenMayaFX -lOpenMayaUI -lImage -lOpenMayaAnim -lOpenMayaRender -lGLU