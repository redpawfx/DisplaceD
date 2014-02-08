#include "DisplaceDeformer.h"
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MItGeometry.h>
#include <maya/MDagModifier.h>
#include <math.h>//added
#include <stdlib.h>//added
#include <maya/MIOStream.h>//added
#include <maya/MFnMatrixAttribute.h>//added
#include <maya/MFnEnumAttribute.h>//added
#include <maya/MFnCompoundAttribute.h>//added
#include <maya/MGlobal.h>//added
#include <maya/MFnMesh.h>//added
#include <maya/MFnComponent.h>//added
#include <maya/MPlugArray.h>//added
#include <iostream>//added
#include <maya/MDagPath.h>//add
#include <maya/M3dView.h>//add
#include <maya/MFloatMatrix.h>//add
#include <maya/MRenderUtil.h>//add
#include <maya/MFloatPointArray.h>//add
#include <maya/MFloatArray.h>//add
#include <maya/MFloatVectorArray.h>//add
#include <maya/MItMeshFaceVertex.h>//add

MTypeId DisplaceDeformer::typeId( 0x0010E244 );
MString DisplaceDeformer::typeName( "displaceD" );

// Attributes
MObject DisplaceDeformer::deformSpace;
MObject	DisplaceDeformer::strength;
MObject	DisplaceDeformer::pointSpace;
MObject	DisplaceDeformer::displaceDirection;
MObject DisplaceDeformer::useUV;
MObject DisplaceDeformer::useNormal;
MObject DisplaceDeformer::color1R;
MObject DisplaceDeformer::color1G;
MObject DisplaceDeformer::color1B;
MObject DisplaceDeformer::color1;
MObject DisplaceDeformer::colorAs;
MObject	DisplaceDeformer::colorOffset;
MObject	DisplaceDeformer::colorOffsetR;
MObject	DisplaceDeformer::colorOffsetG;
MObject	DisplaceDeformer::colorOffsetB;
MObject	DisplaceDeformer::colorMultiply;
MObject	DisplaceDeformer::colorMultiplyR;
MObject	DisplaceDeformer::colorMultiplyG;
MObject	DisplaceDeformer::colorMultiplyB;
MObject DisplaceDeformer::rgbOrder;
MObject DisplaceDeformer::filterSize;

void *DisplaceDeformer::creator()
{
	return new DisplaceDeformer();
}

MStatus DisplaceDeformer::deform( MDataBlock& block, MItGeometry &iter,
												const MMatrix &localToWorld, unsigned int geomIndex )
{
	MStatus stat;
		
	MDataHandle envData = block.inputValue( envelope, &stat );
	if( stat != MS::kSuccess )cerr << "ERROR getting data\n";
	float env = envData.asFloat();	
	if( env == 0.0 ) return MS::kSuccess;

	MDataHandle matData = block.inputValue( deformSpace, &stat );
	if( stat != MS::kSuccess )cerr << "ERROR getting data\n";
	MMatrix mat = matData.asMatrix();
	MMatrix invMat = mat.inverse();

	MDataHandle displaceDirectionData = block.inputValue( displaceDirection, &stat );
	if( stat != MS::kSuccess )cerr << "ERROR getting data\n";
	int directionMode = displaceDirectionData.asShort();

	MDataHandle pointSpaceHnd = block.inputValue( pointSpace, &stat );
	if( stat != MS::kSuccess )cerr << "ERROR getting data\n";
	int pointSpace = pointSpaceHnd.asShort();	

	MDataHandle strengthHnd = block.inputValue( strength, &stat );
	if( stat != MS::kSuccess ) cerr << "ERROR getting data\n";
	float strength = strengthHnd.asFloat();	

	MDataHandle useNormalHnd = block.inputValue( useNormal, &stat );
	if( stat != MS::kSuccess ) cerr << "ERROR getting data\n";
	bool useNormal = useNormalHnd.asBool();	

	MDataHandle useUVHnd = block.inputValue( useUV, &stat );
	if( stat != MS::kSuccess ) cerr << "ERROR getting data\n";
	bool useUV = useUVHnd.asBool();	

	MDataHandle colorAsHnd = block.inputValue( colorAs, &stat );
	if( stat != MS::kSuccess )cerr << "ERROR getting data\n";
	int colorAs = colorAsHnd.asShort();	

	MDataHandle colorOffsetHnd  = block.inputValue( colorOffset, &stat ); 
	if( stat != MS::kSuccess ) cerr << "ERROR getting data\n";
	double3& colorOffset = colorOffsetHnd.asDouble3();

	MDataHandle colorMultiplyHnd  = block.inputValue( colorMultiply, &stat ); 
	if( stat != MS::kSuccess ) cerr << "ERROR getting data\n";
	double3& colorMultiply = colorMultiplyHnd.asDouble3();

	MDataHandle rgbOrderHnd = block.inputValue( rgbOrder, &stat );
	if( stat != MS::kSuccess )cerr << "ERROR getting data\n";
	int rgbOrder = rgbOrderHnd.asShort();

	MDataHandle filterSizeHnd = block.inputValue( filterSize, &stat );
	if( stat != MS::kSuccess ) cerr << "ERROR getting data\n";
	float filterSize = filterSizeHnd.asFloat();	

	/*--------------------------------------------------
	--------------------------------------------------
                    Get colorNode name
	--------------------------------------------------
	--------------------------------------------------*/

	MDataHandle color1Hnd = block.inputValue( color1, &stat );
	if( stat != MS::kSuccess ) return stat;

	MFnDependencyNode depFn(thisMObject());

	MPlug plug0 = depFn.findPlug("color1");

	// will hold the connections to this node
	MPlugArray colorInputPlugs;

	// get input plugs
	plug0.connectedTo(colorInputPlugs,true,false);
	if ( colorInputPlugs.length() != 1 )
		return MS::kSuccess;//valid

	MObject ccc=colorInputPlugs[0].node();

	MFnDependencyNode depFn2(ccc);

	MString colorNode=depFn2.name();
	if(colorNode.length()==0)
		return MS::kSuccess;//valid

	MObject component=iter.component();


	/*--------------------------------------------------
	--------------------------------------------------
                    Get input geometry
	--------------------------------------------------
	--------------------------------------------------*/


	MPlugArray plug;
	depFn.getConnections(plug);
	MObject inputGeometry;

	if(component.apiType()==MFn::kMeshVertComponent){
		//cout << "start finding geometry..." <<endl;

		// check all plug.
		unsigned int j=0;
		for(;j<plug.length();++j) {

			 // get input plugs
			MPlugArray inputPlugs;
			plug[j].connectedTo(inputPlugs,true,false);

			unsigned int k=0;
			for(;k<inputPlugs.length();++k) {

				MObject inputObject=inputPlugs[k].node();

				if(inputObject.apiType()==MFn::kGroupParts){

					MDataHandle inputGeometryHnd = block.inputValue( plug[j], &stat );
					if( stat != MS::kSuccess )
						cerr << "ERROR getting data inputGeometry\n"<< endl;

					inputGeometry=inputGeometryHnd.asMesh();
				}
			}
		}
	}


	/*--------------------------------------------------
	--------------------------------------------------
                    Get Vectors
	--------------------------------------------------
	--------------------------------------------------*/

	MPoint pt;
	float weight;
	float2 uv;
	MFloatArray uCoords,vCoords, filterSizes;
	MFloatPointArray points, refPoints;
	MVector norm;
	MVectorArray vertexNormal;
	MVector tangent;
	MVectorArray vertexTangent;

	int vertId;
	
	if(component.apiType()==MFn::kMeshVertComponent 
			&& inputGeometry.apiType()==MFn::kMeshData){

		/*---------------
		      Polygon
		 ----------------*/

		MFnMesh meshFn( inputGeometry );
		MPointArray pts;
		meshFn.getPoints( pts );

		MIntArray polygonList;			

		//format
		for( unsigned int b=0; b < pts.length(); b++ ){
			points.append( MFloatPoint( 0.0, 0.0, 0.0) );
			refPoints.append( MFloatPoint( 0.0, 0.0, 0.0) );
			uCoords.append(0);
			vCoords.append(0);
			vertexNormal.append( MVector( 0.0, 0.0, 0.0) );
			vertexTangent.append( MVector( 0.0, 0.0, 0.0) );
			//polygonList.append(0);
			filterSizes.append(filterSize);
		}


		//get all vertex based face ID
		if(useNormal && colorAs){
			MIntArray vertexList;
			MVector tmpTangent;
			for(unsigned int i=0;i<meshFn.numPolygons();i++){
				vertexList.clear();
				meshFn.getPolygonVertices(i,vertexList);

				for(unsigned int j=0; j<vertexList.length();j++){
					unsigned int vertexId=vertexList[j];

					meshFn.getFaceVertexTangent (
						i,
						vertexId,
						tmpTangent
					);
					//same value
					if(vertexTangent[vertexId]==tmpTangent)continue;

					if(vertexTangent[vertexId]==MVector(0,0,0)){
						//update
						vertexTangent[vertexId]=tmpTangent;
					}else{
						//might be on border
						//avarage..I know it's just temp code!
						vertexTangent[vertexId]=(vertexTangent[vertexId]+tmpTangent)/2;
					}
				}
			}
		}

		const unsigned int nPts = pts.length();
		bool *visited = new bool[ nPts ];
		unsigned int i;
		for( i=0; i < nPts; i++ )
			visited[i] = false;

		MItMeshFaceVertex fvIter( inputGeometry );

		for( ; !fvIter.isDone(); fvIter.next() )			
		{
			vertId = fvIter.vertId();

			weight = weightValue( block, geomIndex, vertId );
			if( weight == 0.0f )
				continue;
			
			if( visited[ vertId ] )
				continue;
		
			if( !fvIter.hasUVs() )
				continue;

			pt = fvIter.position();//

			//local to World
			if(pointSpace==1)
				pt *=localToWorld;

			pt *= invMat;
			
			//vertex position
	  		points[vertId]=MFloatPoint( pt.x, pt.y, pt.z);
			refPoints[vertId]=MFloatPoint( pt.x, pt.y, pt.z);

			// Get the UV coordinate

			if(useUV){
				fvIter.getUV( uv );
				uCoords[vertId]=uv[0];
				vCoords[vertId]=uv[1];
			}else{
				uCoords[vertId]=pt.x;
				vCoords[vertId]=pt.z;
			}

			//Normal mode
			
			if(useNormal){

				//Looks working well but I'm not sure how this function manages hard edge.
				meshFn.getVertexNormal( vertId, norm );

				/* TANGENT */
				if(colorAs){
					//just ignore in here.
				}else{
					tangent.x=0;
					tangent.y=0;
					tangent.z=0;
				}
			}else if(!directionMode){	

				//angle Mode//
				if(colorAs){

					/* RGB */
					norm.x=1;
					norm.y=0;			
					norm.z=0;
					tangent.x=0;
					tangent.y=1;
					tangent.z=0;

				}else{

					/* luminance */
					norm.x=0;
					norm.y=1;			
					norm.z=0;
				}

			}else{
				//radial mode//
				if(colorAs){
					/* RGB */

					norm.x=pt.x;
					norm.y=pt.y;
					norm.z=pt.z;

					/* TANGENT */
					if(norm==MVector(0,1,0)){
						tangent=MVector(0,0,0);
					}else if(norm==MVector(0,-1,0)){
						tangent=MVector(0,0,0);
					}else{
						tangent=MVector(0,1,0)^norm;tangent.normalize();
					}

				}else{

					/* luminance */
					norm.x=pt.x;
					norm.y=pt.y;
					norm.z=pt.z;

				}

			}


			if(!useNormal){//ignore when normalMode
				norm *= mat;
				tangent *= mat;
			}

			//World to Local
			if(!useNormal && pointSpace==1){//ignore when normalMode
				MMatrix localToWorldInv = localToWorld.inverse();
				//pt *=localToWorldInv;
				norm *=localToWorldInv;
				tangent *=localToWorldInv;
			}

			//norm.normalize();
			vertexNormal[vertId]=norm;
			if(useNormal && colorAs){
				//just ignore in here.
			}else{
				vertexTangent[vertId]=tangent;
			}
			// Tag vertex as having been visited
			visited[ vertId ] = true;

		}
		
		delete[] visited;

	}else{

		/*---------------
		    Non Polygon
		 ----------------*/
		if(useUV)MGlobal::displayWarning("Only Polygon can use \"useUV\" option.");
		if(useNormal)MGlobal::displayWarning("Only Polygon can use \"useNormal\" option.");
		
		//format
		for( unsigned int b=0; b < iter.count(); b++ ){
			points.append( MFloatPoint( 0.0, 0.0, 0.0) );
			refPoints.append( MFloatPoint( 0.0, 0.0, 0.0) );
			uCoords.append(0);
			vCoords.append(0);
			vertexNormal.append( MVector( 0.0, 0.0, 0.0) );
			vertexTangent.append( MVector( 0.0, 0.0, 0.0) );
			filterSizes.append(filterSize);
		}
		
		for( iter.reset(); !iter.isDone(); iter.next() ) 
		{
			vertId =iter.index();

			weight = weightValue( block, geomIndex, vertId );
			if( weight == 0.0f )
				continue;

			pt = iter.position();

			//local to World
			if(pointSpace==1)
				pt *=localToWorld;

			pt *= invMat;
			
			//vertex position
	  		points[vertId]=MFloatPoint( pt.x, pt.y, pt.z);

			//UV

			uCoords[vertId]=pt.x;
			vCoords[vertId]=pt.z;

			//Normal mode

			if(!directionMode){	

				//angle Mode//
				if(colorAs){

					/* RGB */
					norm.x=1;
					norm.y=0;			
					norm.z=0;
					tangent.x=0;
					tangent.y=1;
					tangent.z=0;

				}else{

					/* luminance */
					norm.x=0;
					norm.y=1;			
					norm.z=0;
				}
			}else{

				//radial mode//
				if(colorAs){
					/* RGB */

					norm.x=pt.x;
					norm.y=pt.y;
					norm.z=pt.z;

					/* TANGENT */
					if(norm==MVector(0,1,0)){
						tangent=MVector(0,0,0);
					}else if(norm==MVector(0,-1,0)){
						tangent=MVector(0,0,0);
					}else{
						tangent=MVector(0,1,0)^norm;tangent.normalize();
					}

				}else{

					/* luminance */
					norm.x=pt.x;
					norm.y=pt.y;
					norm.z=pt.z;

				}
			}
			
			norm *= mat;
			tangent *= mat;

			//World to Local
			if(pointSpace==1){
				MMatrix localToWorldInv = localToWorld.inverse();
				norm *=localToWorldInv;
				tangent *=localToWorldInv;
			}

			vertexNormal[vertId]=norm;
			vertexTangent[vertId]=tangent;
		}
	}

	//check empty
	if( vertexNormal.length()==0 ){
		cout << "Can't get any normals..." <<endl;
		return stat;
	}
	
	/*--------------------------------------------------
	--------------------------------------------------
                         MRenderUtil
	 --------------------------------------------------
	--------------------------------------------------*/
	
	bool shadow = 0;
	bool reuse = 0;
	MFloatVectorArray colors, transps;

	// get current camera
	MDagPath cameraPath;
	M3dView::active3dView().getCamera( cameraPath );
	MMatrix camMat = cameraPath.inclusiveMatrix();
	MFloatMatrix cameraMat( camMat.matrix );

	stat = MRenderUtil::sampleShadingNetwork(
		(colorNode+".outColor"),
		points.length(),
		shadow,
		reuse,
		cameraMat,
		( points.length()>0)?&points : NULL,
		(uCoords.length()>0)?&uCoords : NULL,
		(vCoords.length()>0)?&vCoords : NULL,
		NULL,
		( points.length()>0)?&points : NULL,
		NULL,
		NULL,
		&filterSizes,
		colors,
		transps );
	if( stat != MS::kSuccess ) return stat;


	/*--------------------------------------------------
	--------------------------------------------------
                    Apply value to vertex
	--------------------------------------------------
	--------------------------------------------------*/


	if(vertexNormal.length()<iter.count() || colors.length()<iter.count())
		cerr << "ERROR Number of Array!\n"<< endl;

	for( iter.reset(); !iter.isDone(); iter.next() ) 
	{	

		weight = weightValue( block, geomIndex, iter.index() );
		if( weight == 0.0f )
			continue;

		pt = iter.position();


		if(colorAs){
			norm=vertexNormal[iter.index()];norm.normalize();
			tangent=vertexTangent[iter.index()];tangent.normalize();
			MVector biNormal=MVector(0,0,0);
			if(tangent!=MVector(0,0,0))biNormal=norm^tangent;biNormal.normalize();

			switch(rgbOrder){
			case 0:
				//XYZ
				pt += env * strength * weight * (colors[iter.index()].x+colorOffset[0])* colorMultiply[0] * norm;
				pt += env * strength * weight * (colors[iter.index()].y+colorOffset[1])* colorMultiply[1] * tangent;
				pt += env * strength * weight * (colors[iter.index()].z+colorOffset[2])* colorMultiply[2] * biNormal;
				break;
			case 1:
				//YZX
				pt += env * strength * weight * (colors[iter.index()].z+colorOffset[2])* colorMultiply[2] * norm;
				pt += env * strength * weight * (colors[iter.index()].x+colorOffset[0])* colorMultiply[0] * tangent;
				pt += env * strength * weight * (colors[iter.index()].y+colorOffset[1])* colorMultiply[1] * biNormal;
				break;
			case 2:
				//ZXY
				pt += env * strength * weight * (colors[iter.index()].y+colorOffset[1])* colorMultiply[1] * norm;
				pt += env * strength * weight * (colors[iter.index()].z+colorOffset[2])* colorMultiply[2] * tangent;
				pt += env * strength * weight * (colors[iter.index()].x+colorOffset[0])* colorMultiply[0] * biNormal;
				break;
			case 3:
				//XZY
				pt += env * strength * weight * (colors[iter.index()].x+colorOffset[0])* colorMultiply[0] * norm;
				pt += env * strength * weight * (colors[iter.index()].z+colorOffset[2])* colorMultiply[2] * tangent;
				pt += env * strength * weight * (colors[iter.index()].y+colorOffset[1])* colorMultiply[1] * biNormal;
				break;
			case 4:
				//YXZ
				pt += env * strength * weight * (colors[iter.index()].y+colorOffset[1])* colorMultiply[1] * norm;
				pt += env * strength * weight * (colors[iter.index()].x+colorOffset[0])* colorMultiply[0] * tangent;
				pt += env * strength * weight * (colors[iter.index()].z+colorOffset[2])* colorMultiply[2] * biNormal;
				break;
			case 5:
				//ZYX
				pt += env * strength * weight * (colors[iter.index()].z+colorOffset[2])* colorMultiply[2] * norm;
				pt += env * strength * weight * (colors[iter.index()].y+colorOffset[1])* colorMultiply[1] * tangent;
				pt += env * strength * weight * (colors[iter.index()].x+colorOffset[0])* colorMultiply[0] * biNormal;
				break;
			}
		}else{
			/* get color brightness */
				/*
				float brightness=0;
				if(colors[iter.index()].x>brightness)brightness=colors[iter.index()].x;
				if(colors[iter.index()].y>brightness)brightness=colors[iter.index()].y;
				if(colors[iter.index()].z>brightness)brightness=colors[iter.index()].z;
				*/
			/* get color Luminance */
			float luminance = ( 0.298912 * colors[iter.index()].x + 0.586611 * colors[iter.index()].y + 0.114478 * colors[iter.index()].z );

			norm=vertexNormal[iter.index()];norm.normalize();
			pt += env * strength * weight * luminance * norm;
		}

		iter.setPosition( pt );

	}

	return stat;
}

MObject &DisplaceDeformer::accessoryAttribute() const
{
	return deformSpace;
}

MStatus DisplaceDeformer::accessoryNodeSetup( MDagModifier &dagMod )
{
	MStatus stat;

	//createNode
	MObject transObj=dagMod.createNode( "transform", MObject::kNullObj, &stat );
	if( !stat )	return stat;
	dagMod.renameNode( transObj, "displaceDhandle" );
	dagMod.doIt ();

	MObject shapeObj = dagMod.createNode( "DisplaceDhandle", transObj, &stat );
	if( !stat )	return stat;
	dagMod.renameNode( shapeObj, "displaceDhandleShape" );


	//connectAttr
	MFnDependencyNode transFn( transObj );
	// use worldMatrix so that you can group and transform locator node.
	//MObject attrMat = transFn.attribute( "matrix" );
	MObject attrMat = transFn.attribute( "worldMatrix" );
	stat = dagMod.connect( transObj, attrMat, thisMObject(), deformSpace );
	if( !stat )	return stat;
	dagMod.doIt ();

	MFnDependencyNode shapeFn(shapeObj);
	MObject attrUseUV= shapeFn.attribute( "useUV" );
	stat = dagMod.connect(  thisMObject(), useUV, shapeObj, attrUseUV );
	dagMod.doIt ();

	MObject attrUseNormal= shapeFn.attribute( "useNormal" );
	stat = dagMod.connect(  thisMObject(), useNormal, shapeObj, attrUseNormal );
	dagMod.doIt ();

	MObject attrDisplaceDirection= shapeFn.attribute( "displaceDirection" );
	stat = dagMod.connect(  thisMObject(), displaceDirection, shapeObj, attrDisplaceDirection );
	//dagMod.doIt ();

	return stat;
}

MStatus DisplaceDeformer::initialize()
{
	MFnMatrixAttribute mAttr;
	MFnNumericAttribute nAttr;
	//MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;
	MFnCompoundAttribute cAttr;
	MStatus stat;

	// Set up inputs
	deformSpace = mAttr.create( "deformSpace", "dSp" );
	mAttr.setStorable( false );
	mAttr.setHidden( true );

	strength = nAttr.create( "strength", "st", MFnNumericData::kFloat, 1.0 );
 	nAttr.setStorable(true);
	nAttr.setKeyable( true );

	pointSpace = eAttr.create("pointSpace", "ps", 0);
	eAttr.addField("local", 0);
	eAttr.addField("world", 1);
 	eAttr.setStorable(true);
 	eAttr.setKeyable( true );

	displaceDirection = eAttr.create("displaceDirection", "nd", 0);
	eAttr.addField("locatorAngle", 0);
	eAttr.addField("locatorRadial", 1);
 	eAttr.setStorable(true);
 	eAttr.setKeyable( true );

	useUV = nAttr.create("useUV", "uu",MFnNumericData::kBoolean, 0);
 	nAttr.setStorable(true);
 	nAttr.setKeyable( true );

	useNormal = nAttr.create("useNormal", "un",MFnNumericData::kBoolean, 0);
 	nAttr.setStorable(true);
 	nAttr.setKeyable( true );

	/* color1 */
	color1R = nAttr.create( "color1R", "c1r",MFnNumericData::kFloat);
	nAttr.setKeyable( true ); 
	nAttr.setStorable( true );

	color1G = nAttr.create( "color1G", "c1g",MFnNumericData::kFloat);
	nAttr.setKeyable( true );
	nAttr.setStorable( true );

	color1B = nAttr.create( "color1B", "c1b",MFnNumericData::kFloat);
	nAttr.setKeyable( true );
	nAttr.setStorable(true);

	color1 = nAttr.create( "color1","c1",color1R,color1G,color1B);
	nAttr.setKeyable( true );
	nAttr.setStorable( true );
	nAttr.setUsedAsColor( true) ;

	/* colorOffset */
	colorOffsetR = nAttr.create( "colorOffsetR", "coR", MFnNumericData::kDouble, 0.0 );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);

	colorOffsetG = nAttr.create( "colorOffsetG", "coG", MFnNumericData::kDouble, 0.0 );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);

	colorOffsetB = nAttr.create( "colorOffsetB", "coB", MFnNumericData::kDouble, 0.0 );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);

	colorOffset = nAttr.create( "colorOffset", "co", colorOffsetR, colorOffsetG, colorOffsetB );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);
	nAttr.setDefault( 0.0, 0.0, 0.0 );


	/* colorMultiply */
	colorMultiplyR = nAttr.create( "colorMultiplyR", "cmR", MFnNumericData::kDouble, 1.0 );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);

	colorMultiplyG = nAttr.create( "colorMultiplyG", "cmG", MFnNumericData::kDouble, 1.0 );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);

	colorMultiplyB = nAttr.create( "colorMultiplyB", "cmB", MFnNumericData::kDouble, 1.0 );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);
	
	colorMultiply = nAttr.create( "colorMultiply", "cm", colorMultiplyR, colorMultiplyG, colorMultiplyB );
	nAttr.setKeyable( true );
	nAttr.setStorable(true);
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	
	
	colorAs = eAttr.create("colorAs", "ca", 0);
	eAttr.addField("luminance", 0);
	eAttr.addField("RGB", 1);
 	eAttr.setStorable(true);
 	eAttr.setKeyable( true );
	
	
	rgbOrder = eAttr.create("rgbOrder", "rgbo", 0);
	eAttr.addField("XYZ", 0);
	eAttr.addField("YZX", 1);
	eAttr.addField("ZXY", 2);
	eAttr.addField("XZY", 3);
	eAttr.addField("YXZ", 4);
	eAttr.addField("ZYX", 5);
 	eAttr.setStorable(true);
 	eAttr.setKeyable( true );

	filterSize = nAttr.create( "filterSize", "fs", MFnNumericData::kFloat, 0.0 );
 	nAttr.setStorable(true);
	nAttr.setKeyable( true );
	
	//addAttribute
	stat = addAttribute( deformSpace );
		if (!stat) { stat.perror("addAttribute deformSpace"); return stat;}
	stat = addAttribute( strength );
		if (!stat) { stat.perror("addAttribute strength"); return stat;}
	stat = addAttribute(pointSpace);
		if (!stat) { stat.perror("addAttribute pointSpace"); return stat;}
	stat = addAttribute(useUV);
		if (!stat) { stat.perror("addAttribute useUV"); return stat;}
	stat = addAttribute(useNormal);
		if (!stat) { stat.perror("addAttribute useNormal"); return stat;}
	stat = addAttribute(displaceDirection);
		if (!stat) { stat.perror("addAttribute displaceDirection"); return stat;}
	stat =addAttribute(color1);
		if (!stat) { stat.perror("addAttribute color1"); return stat;}
	stat = addAttribute(colorAs);
		if (!stat) { stat.perror("addAttribute colorAs"); return stat;}
	stat =addAttribute(colorOffset);
		if (!stat) { stat.perror("addAttribute colorOffset"); return stat;}
	stat =addAttribute(colorMultiply);
		if (!stat) { stat.perror("addAttribute colorMultiply"); return stat;}
	stat = addAttribute(rgbOrder);
		if (!stat) { stat.perror("addAttribute rgbOrder"); return stat;}
	stat = addAttribute( filterSize );
		if (!stat) { stat.perror("addAttribute filterSize"); return stat;}

	//attributeAffects
	stat = attributeAffects( deformSpace, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( strength, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( pointSpace, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( useUV, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( useNormal, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( displaceDirection, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat =attributeAffects( color1R, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat =attributeAffects( color1G, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat =attributeAffects( color1B, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat =attributeAffects( color1, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorAs, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorOffsetR, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorOffsetG, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorOffsetB, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorOffset, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorMultiplyR, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorMultiplyG, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorMultiplyB, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( colorMultiply, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( rgbOrder, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( filterSize, outputGeom );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	return MS::kSuccess;
}
