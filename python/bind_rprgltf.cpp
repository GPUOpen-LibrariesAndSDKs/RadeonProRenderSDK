#include "bind_common.h"



NB_MODULE(rprgltf, m) {

	m.attr("RPRGLTF_EXPORTFLAG_GLB") = (1 << 0)  ;
	m.attr("RPRGLTF_EXPORTFLAG_NO_TEXTUREFLIPV") = (1 << 1)  ;
	m.attr("RPRGLTF_EXPORTFLAG_COPY_IMAGES_USING_OBJECTNAME") = (1 << 2)  ;
	m.attr("RPRGLTF_EXPORTFLAG_KHR_LIGHT") =  (1 << 3)  ;
	m.attr("RPRGLTF_EXPORTFLAG_BUILD_STD_PBR_IMAGES") =  (1 << 4)  ;
	m.attr("RPRGLTF_IMPORTFLAG_NO_TEXTUREFLIPV") =  (1 << 0)  ;
	 

	m.def("Export", []( std::string rprsFileName,  PyRprContext* context,  PyRprScene* scene,  rpr_uint exportFlags,  PyRprsContext* rprsCtx )
	{

		PYRPR_CHECK_ERROR( rprExportToGLTF(
			rprsFileName.c_str(),  
			context->h,  
			nullptr,
			&scene->h,  
			1, 
			exportFlags,  
			rprsCtx->h   )  );
		return ret;
	});
	m.def("Import", []( std::string rprsFileName,  PyRprContext* context,  PyRprMaterialSystem* materialSystem,  PyRprScene* scene,  rpr_uint importFlags,  PyRprsContext* rprsCtx )
	{

		PYRPR_CHECK_ERROR( rprImportFromGLTF(
			rprsFileName.c_str(),  
			context->h,  
			materialSystem->h,
			&(scene->h),  
			nullptr,
			importFlags,  
			rprsCtx->h   )  );

		return ret;
	});





	m.def("AddExtraCamera", []( PyRprCamera* extraCam )
	{

		PYRPR_CHECK_ERROR( rprGLTF_AddExtraCamera(  extraCam->h   )  );
		return ret;
	});

	m.def("AddExtraShapeParameter", []( PyRprShape* shape,  std::string parameterName,  rpr_int value )
	{

		PYRPR_CHECK_ERROR( rprGLTF_AddExtraShapeParameter(  shape->h,  parameterName.c_str(),  value   )  );
		return ret;
	});


	m.def("AssignShapeToGroup", []( PyRprShape* shape,  std::string groupName )
	{

		PYRPR_CHECK_ERROR( rprGLTF_AssignShapeToGroup(  shape->h,  groupName.c_str()   )  );
		return ret;
	});

	m.def("AssignLightToGroup", []( PyRprLight* light,  std::string groupName )
	{

		PYRPR_CHECK_ERROR( rprGLTF_AssignLightToGroup(  light->h,  groupName.c_str()   )  );
		return ret;
	});

	m.def("AssignCameraToGroup", []( PyRprCamera* camera,  std::string groupName )
	{

		PYRPR_CHECK_ERROR( rprGLTF_AssignCameraToGroup(  camera->h,  groupName.c_str()   )  );
		return ret;
	});

	m.def("AssignParentGroupToGroup", []( std::string groupChild,  std::string groupParent )
	{

		PYRPR_CHECK_ERROR( rprGLTF_AssignParentGroupToGroup(  groupChild.c_str(),  groupParent.c_str()   )  );
		return ret;
	});

	m.def("SetTransformGroup", []( std::string groupChild,  nb::ndarray<float_t, nb::shape<nb::any>, nb::c_contig, nb::device::cpu> matrixComponents )
	{
		const rpr_float* matrixComponents___pycast =    matrixComponents.size() == 0 ? nullptr :      &(((rpr_float*)matrixComponents.data())[0]);

		PYRPR_CHECK_ERROR( rprGLTF_SetTransformGroup(  groupChild.c_str(),  matrixComponents___pycast   )  );
		return ret;
	});


	m.def("ReleaseImportedData", [](  )
	{

		PYRPR_CHECK_ERROR( rprGLTF_ReleaseImportedData(     )  );
		return ret;
	});
	m.def("DeleteListImportedObjects", [](  )
	{

		PYRPR_CHECK_ERROR( rprGLTF_DeleteListImportedObjects(     )  );
		return ret;
	});

	m.attr("RPRGLTF_ANIMATION_MOVEMENTTYPE_TRANSLATION") = 0x1 ;
	m.attr("RPRGLTF_ANIMATION_MOVEMENTTYPE_ROTATION") = 0x2 ;
	m.attr("RPRGLTF_ANIMATION_MOVEMENTTYPE_SCALE") = 0x3 ;



	RPRPY_ADDITIONAL_DEFINES_rprgltf;


}
