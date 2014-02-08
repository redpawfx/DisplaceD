#ifndef DISPLACEDEFORMER_H
#define DISPLACEDEFORMER_H

#include <maya/MPxLocatorNode.h> 
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/M3dView.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MPxManipContainer.h>
#include <maya/MPointArray.h>
#include <maya/MPxDeformerNode.h>
#include <math.h>
#include <maya/MIOStream.h>
#include <maya/MFloatPoint.h>//added

class DisplaceDeformer : public MPxDeformerNode
{
public:
	static  void *creator();
	static  MStatus initialize();

    virtual MStatus deform( MDataBlock &block,
									   MItGeometry &iter,
									   const MMatrix &mat,
									   unsigned int multiIndex );

	virtual MObject &accessoryAttribute() const;
	virtual MStatus accessoryNodeSetup( MDagModifier &cmd );

	static MTypeId typeId;
	static MString typeName;

private:

	// Attributes
	static MObject deformSpace;
	static MObject strength;
	static MObject useUV;
	static MObject useNormal;
	static MObject pointSpace;
	static MObject displaceDirection;
	static MObject color1R,color1G,color1B,color1;
	static MObject colorAs;
	static MObject colorOffsetR,colorOffsetG,colorOffsetB,colorOffset;
	static MObject colorMultiplyR,colorMultiplyG,colorMultiplyB,colorMultiply;
	static MObject rgbOrder;
	static MObject filterSize;
};

#endif
