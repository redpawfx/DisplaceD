cd /home/hajime/Dropbox/data/Develop/Maya/Mll/projects/displacementD/linux
export version=2008
export MAYA_LOCATION=/usr/autodesk/maya$version-x64
export LD_LIBRARY_PATH=$MAYA_LOCATION/lib

gcc -fPIC -c -I. -I.. -I$MAYA_LOCATION/include -I/usr/X11R6/include -m64 -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM -Wno-deprecated -fno-gnu-keywords ../DisplaceDeformer.cpp ../DisplaceDhandle.cpp ../PluginMain.cpp
gcc -shared -m64  -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic -o ../outputs/linux64/$version/DisplaceD.so DisplaceDeformer.o DisplaceDhandle.o PluginMain.o -L$MAYA_LOCATION/lib -lFoundation -lOpenMaya -lOpenMayaFX -lOpenMayaUI -lImage -lOpenMayaAnim -lOpenMayaRender -lGLU
