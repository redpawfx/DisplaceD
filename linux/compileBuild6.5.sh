export version=6.5
export gccDir=/usr/local/gcc334/bin/gcc
export workDir=/huge/develop/mll/projects/displacementD
export MAYA_LOCATION=/usr/aw/maya$version
export LD_LIBRARY_PATH=$MAYA_LOCATION/lib

cd $workDir/linux


$gccDir -c -I. -I.. -I$MAYA_LOCATION/include -I/usr/X11R6/include -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM -mcpu=pentium4 -Wno-deprecated -fno-gnu-keywords ../DisplaceDeformer.cpp ../DisplaceDhandle.cpp ../PluginMain.cpp
$gccDir -shared  -O3 -pthread -pipe -D_BOOL -DLINUX -DREQUIRE_IOSTREAM  -mcpu=pentium4  -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic -o $workDir/outputs/linux32/$version/DisplaceD.so DisplaceDeformer.o DisplaceDhandle.o PluginMain.o -L$MAYA_LOCATION/lib -lFoundation -lOpenMaya -lOpenMayaFX -lOpenMayaUI -lImage -lOpenMayaAnim -lOpenMayaRender -lGLU
