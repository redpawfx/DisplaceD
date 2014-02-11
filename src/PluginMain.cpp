#include "DisplaceDhandle.h"
#include "DisplaceDeformer.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{ 
	MStatus stat;
	MString errStr;
	MFnPlugin plugin( obj, "Hajime Nakamura", "1.13", "Any");

	stat = plugin.registerNode( DisplaceDhandle::typeName,
				DisplaceDhandle::typeId,
				&DisplaceDhandle::creator, &DisplaceDhandle::initialize, 
				MPxNode::kLocatorNode );
	if ( !stat )
	{
		errStr = "registerNode failed";
		goto error;
	}


	stat = plugin.registerNode( DisplaceDeformer::typeName, 
				DisplaceDeformer::typeId, 
				DisplaceDeformer::creator, 
				DisplaceDeformer::initialize, 
				MPxNode::kDeformerNode );
	if ( !stat )
	{
		errStr = "registerNode failed";
		goto error;
	}

	stat = plugin.registerUI("displaceDCreateUI", "displaceDDeleteUI");
	if ( !stat )
	{
		errStr = "addMenu failed";
		goto error;
	}

	return stat;

	error:

	stat.perror( errStr );
	return stat;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus stat;
	MString errStr;
	MFnPlugin plugin( obj );

	stat = plugin.deregisterNode( DisplaceDhandle::typeId );
	if( !stat ) 
	{
		errStr = "deregisterNode failed";
		goto error;
	}

	stat = plugin.deregisterNode( DisplaceDeformer::typeId );
	if( !stat ) 
	{
		errStr = "deregisterNode failed";
		goto error;
	}

	return stat;

	error:

	stat.perror( errStr );
	return stat;
}
