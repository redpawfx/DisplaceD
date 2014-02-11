#include "DisplaceDhandle.h"
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnEnumAttribute.h>//added
//#include <maya/MColor.h>

const MTypeId DisplaceDhandle::typeId( 0x0010E245 );
const MString DisplaceDhandle::typeName( "DisplaceDhandle" );

MObject	DisplaceDhandle::useUV;
MObject	DisplaceDhandle::useNormal;
MObject	DisplaceDhandle::displaceDirection;

void DisplaceDhandle::draw( M3dView & view, const MDagPath & path,
M3dView::DisplayStyle style, M3dView::DisplayStatus status )
{ 
	//getAttr
	int directionMode;
	MFnDagNode fn( thisMObject()  );
	MPlug displaceDirectionPlug  = fn.findPlug( displaceDirection );
	displaceDirectionPlug.getValue( directionMode );

	int useUVmode;
	MPlug useUVplug  = fn.findPlug( useUV );
	useUVplug.getValue( useUVmode );

	int useNormalMode;
	MPlug useNormalPlug  = fn.findPlug( useNormal );
	useNormalPlug.getValue( useNormalMode );

	//	MColor col = colorRGB( status );

	view.beginGL(); 
	glPushAttrib( GL_CURRENT_BIT );

	if(status == M3dView::kLead)
		//select
		glColor3f(0.263, 1.00, 0.639);
	else
		glColor3f(0.228, 0.432, 0.647);
		//default
		//glColor3f(col.r,col.g,col.b);


	if(useUVmode==0){
		//plane
		glBegin(GL_LINE_STRIP);
			glVertex3f( 1.0f, 0.0f, 1.0f );
			glVertex3f( -1.0f, 0.0f, 1.0f );
			glVertex3f( -1.0f, 0.0f, -1.0f );
			glVertex3f( 1.0f, 0.0f, -1.0f );
			glVertex3f( 1.0f, 0.0f, 1.0f );
		glEnd();

		glBegin(GL_LINES);
			glVertex3f( 1.0f, 0.0f, 0.0f );
			glVertex3f( -1.0f, 0.0f, 0.0f );

			glVertex3f( 0.0f, 0.0f, 1.0f );
			glVertex3f( 0.0f, 0.0f, -1.0f );
		glEnd();
	}else{

		//UV
		
		glBegin(GL_LINE_STRIP);
			glVertex3f( -0.325f, 0.275f, 0.0f );
			glVertex3f( -0.325f, 0.075f, 0.0f );
			glVertex3f( -0.2833050216f, 0.02183412939f, 0.0f );
			glVertex3f( -0.2f, 0.0f, 0.0f );
			glVertex3f( -0.1166949784f, 0.02183412939f, 0.0f );
			glVertex3f( -0.075f, 0.075f, 0.0f );
			glVertex3f( -0.075f, 0.275f, 0.0f );
			glVertex3f( -0.125f, 0.275f, 0.0f );
			glVertex3f( -0.125f, 0.1f, 0.0f );
			glVertex3f( -0.1486844797f, 0.06550238817f, 0.0f );
			glVertex3f( -0.2f, 0.05f, 0.0f );
			glVertex3f( -0.2513155203f, 0.06550238817f, 0.0f );
			glVertex3f( -0.275f, 0.1f, 0.0f );
			glVertex3f( -0.275f, 0.275f, 0.0f );
			glVertex3f( -0.325f, 0.275f, 0.0f );
		glEnd();
		
		glBegin(GL_LINE_STRIP);
			glVertex3f( 0.075f, 0.275f, 0.0f );
			glVertex3f( 0.175f, 0.0f, 0.0f );
			glVertex3f( 0.225f, 0.0f, 0.0f );
			glVertex3f( 0.325f, 0.275f, 0.0f );
			glVertex3f( 0.275f, 0.275f, 0.0f );
			glVertex3f( 0.2f, 0.05f, 0.0f );
			glVertex3f( 0.125f, 0.275f, 0.0f );
			glVertex3f( 0.075f, 0.275f, 0.0f );
		glEnd();
	}

	if(useNormalMode==1){

		glBegin(GL_LINE_STRIP);
			glVertex3f( -0.130093601f, -0.04352691531f, 0.0f );
			glVertex3f( -0.05322140732f, -0.04352691531f, 0.0f );
			glVertex3f( 0.05355358285f, -0.2076917887f, 0.0f );
			glVertex3f( 0.05355358285f, -0.04352691531f, 0.0f );
			glVertex3f( 0.1308787129f, -0.04352691531f, 0.0f );
			glVertex3f( 0.1308787129f, -0.3141648705f, 0.0f );
			glVertex3f( 0.05355358285f, -0.3141648705f, 0.0f );
			glVertex3f( -0.05352346322f, -0.1506039614f, 0.0f );
			glVertex3f( -0.05276847096f, -0.3141648705f, 0.0f );
			glVertex3f( -0.130093601f, -0.3141648705f, 0.0f );
			glVertex3f( -0.130093601f, -0.04352691531f, 0.0f );
		glEnd();

	}else{

		if(directionMode==0){
			//directional
			glBegin(GL_LINE_STRIP);
				glVertex3f( 0.0f, 0.0f, 0.0f );
				glVertex3f( 0.0f, 1.0f, 0.0f );
				glVertex3f( -0.07f, 0.8f, 0.0f );
				glVertex3f( 0.07f, 0.8f, 0.0f );
				glVertex3f( 0.0f, 1.0f, 0.0f );
			glEnd();
		}else{
			//radial
			glBegin(GL_LINE_STRIP);
				//up
				glVertex3f( 0.0f, 0.0f, 0.0f );
				glVertex3f( 0.0f, 1.0f, 0.0f );
				glVertex3f( -0.07f, 0.8f, 0.0f );
				glVertex3f( 0.07f, 0.8f, 0.0f );
				glVertex3f( 0.0f, 1.0f, 0.0f );
			glEnd();

			glBegin(GL_LINE_STRIP);
				//down
				glVertex3f( 0.0f, 0.0f, 0.0f );
				glVertex3f( 0.0f, -1.0f, 0.0f );
				glVertex3f( -0.07f, -0.8f, 0.0f );
				glVertex3f( 0.07f, -0.8f, 0.0f );
				glVertex3f( 0.0f, -1.0f, 0.0f );
			glEnd();

			glBegin(GL_LINE_STRIP);
				//Up Right
				glVertex3f( 0.0f, 0.0f, 0.0f );
				glVertex3f( 0.7f, 0.7f, 0.0f );
				glVertex3f( 0.5f, 0.6f, 0.0f );
				glVertex3f( 0.6f, 0.5f, 0.0f );
				glVertex3f( 0.7f, 0.7f, 0.0f );
			glEnd();

			glBegin(GL_LINE_STRIP);
				//Up Left
				glVertex3f( -0.0f, 0.0f, 0.0f );
				glVertex3f( -0.7f, 0.7f, 0.0f );
				glVertex3f( -0.5f, 0.6f, 0.0f );
				glVertex3f( -0.6f, 0.5f, 0.0f );
				glVertex3f( -0.7f, 0.7f, 0.0f );
			glEnd();

			glBegin(GL_LINE_STRIP);
				//Down Right
				glVertex3f( 0.0f, -0.0f, 0.0f );
				glVertex3f( 0.7f, -0.7f, 0.0f );
				glVertex3f( 0.5f, -0.6f, 0.0f );
				glVertex3f( 0.6f, -0.5f, 0.0f );
				glVertex3f( 0.7f, -0.7f, 0.0f );
			glEnd();

			glBegin(GL_LINE_STRIP);
				//Down Left
				glVertex3f( -0.0f, -0.0f, 0.0f );
				glVertex3f( -0.7f, -0.7f, 0.0f );
				glVertex3f( -0.5f, -0.6f, 0.0f );
				glVertex3f( -0.6f, -0.5f, 0.0f );
				glVertex3f( -0.7f, -0.7f, 0.0f );
			glEnd();
		}
	}
	glPopAttrib();
	view.endGL();       
}

bool DisplaceDhandle::isBounded() const
{ 
	return true;
}

MBoundingBox DisplaceDhandle::boundingBox() const
//
// N.B. It is important to have this bounding box function otherwise zoom selected and 
// zoom all won't work correctly.
//
{   
	MPointArray pts;
	MPoint pt;
	pt.x=1.0;
	pt.y=1.0;
	pt.z=1.0;
	pts.append(pt);
	pt.x=-1.0;
	pt.y=-1.0;
	pt.z=-1.0;
	pts.append(pt);

	MBoundingBox bbox;
	for( unsigned int i=0; i < pts.length(); i++ )
		bbox.expand( pts[i] );
	return bbox;
}


void *DisplaceDhandle::creator()
{
	return new DisplaceDhandle();
}

MStatus DisplaceDhandle::initialize()
{    
	MStatus stat;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	useUV = nAttr.create("useUV", "uu",MFnNumericData::kBoolean, 0);
	useNormal = nAttr.create("useNormal", "un",MFnNumericData::kBoolean, 0);

	displaceDirection = eAttr.create("displaceDirection", "nd", 0);
	eAttr.addField("locatorAngle", 0);
	eAttr.addField("locatorRadial", 1);

	stat = addAttribute(useUV);
		if (!stat) { stat.perror("addAttribute useUV"); return stat;}
	stat = addAttribute(useNormal);
		if (!stat) { stat.perror("addAttribute useNormal"); return stat;}
	stat = addAttribute(displaceDirection);
		if (!stat) { stat.perror("addAttribute displaceDirection"); return stat;}

	return MS::kSuccess;
}
