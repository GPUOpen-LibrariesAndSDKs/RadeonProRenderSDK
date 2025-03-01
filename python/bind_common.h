#pragma once


#include <nanobind/nanobind.h>
#include <RadeonProRender.h>
#include <RprLoadStore.h>
#include <ProRenderGLTF.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/ndarray.h>


namespace nb = nanobind;


// generates:   const rpr_float** a___pycast    from a 2-dimensions ndarray-float
#define RPRPY_NDARRY_TO_PPFLOAT(a,o)\
		const rpr_float* a##___pycast1 =  &(((rpr_float*)a.data())[o]);\
		std::vector<const rpr_float*> a##___vec;\
		for(int i=0; i<a.shape(0); i++)\
			a##___vec.push_back(&(a##___pycast1[  a.shape(1)*i  ]));\
		const rpr_float** a##___pycast = &(a##___vec[0]);


// generates:   const int32_t** a___pycast    from a 2-dimensions ndarray-int32_t
#define RPRPY_NDARRY_TO_PPINT32(a,o)\
		const int32_t* a##___pycast1 =  &(((int32_t*)a.data())[o]);\
		std::vector<const int32_t*> a##___vec;\
		for(int i=0; i<a.shape(0); i++)\
			a##___vec.push_back(&(a##___pycast1[  a.shape(1)*i  ]));\
		const int32_t** a##___pycast = &(a##___vec[0]);





void RprError(int code);

#define PYRPR_CHECK_ERROR(a)   rpr_status ret = a; if ( ret != RPR_SUCCESS ) { RprError(ret); }

struct PyRprScene { void* h = nullptr; };
struct PyRprCamera { void* h = nullptr; };
struct PyRprShape { void* h = nullptr; };
struct PyRprLight { void* h = nullptr; };
struct PyRprFramebuffer { void* h = nullptr; };
struct PyRprContext { void* h = nullptr; };
struct PyRprsContext { void* h = nullptr; };
struct PyRprImage { void* h = nullptr; };
struct PyRprBuffer { void* h = nullptr; };
struct PyRprHeteroVolume { void* h = nullptr; };
struct PyRprGrid { void* h = nullptr; };
struct PyRprCurve { void* h = nullptr; };
struct PyRprMaterialSystem { void* h = nullptr; };
struct PyRprMaterialNode { void* h = nullptr; };
struct PyRprPostEffect { void* h = nullptr; };
struct PyRprComposite { void* h = nullptr; };
struct PyRprLut { void* h = nullptr; };


struct PyRprsAnimation
{
    uint32_t structSize;
    std::string groupName;
    uint32_t movementType;
    uint32_t interpolationType;
    uint32_t nbTimeKeys;
    uint32_t nbTransformValues;
    std::vector<float> timeKeys;
    std::vector<float> transformValues;
};

struct PyRprgltfAnimation
{
    uint32_t structSize;
    std::string groupName;
    uint32_t movementType;
    uint32_t interpolationType;
    uint32_t nbTimeKeys;
    uint32_t nbTransformValues;
    std::vector<float> timeKeys;
    std::vector<float> transformValues;
};




struct PyMalloc
{
	PyMalloc( nb::ndarray<> a);
	PyMalloc( std::string str);

	// delete the allocated pointers here ( instead of inside the destructor )
	void Free();

	// DO NOT delete the allocated pointers here
	// this is the responsability of the user on Python side to call Free().
	// If the pointers were released here ( inside the destructor ) we would have a risk of having undefined pointer on the Python side.
	// for example if in the python script we do :
	//     myBuffer = rpr.PyMalloc("text").GetMem(0)
	// in this script,  ~PyMalloc()  will be called during the execution of this line. so myBuffer would hold an invalid pointer if the string were deleted inside  ~PyMalloc()
	~PyMalloc();

	void* GetMem(int offset);

	nb::ndarray<>* m_data_asNdarray;
	nb::dlpack::dtype m_dtype;
	std::string* m_data_asString;
};



#define RPRPY_ADDITIONAL_DEFINES_rpr\
\
	m.def("ObjectDelete", []( PyRprScene* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprCamera* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprShape* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprLight* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprFramebuffer* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprContext* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprImage* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprBuffer* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprHeteroVolume* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprGrid* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprCurve* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprMaterialSystem* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprMaterialNode* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprPostEffect* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprComposite* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
	m.def("ObjectDelete", []( PyRprLut* obj ) {   PYRPR_CHECK_ERROR( rprObjectDelete(  obj->h   )  );   return ret;   });\
\
	nb::class_<PyRprScene>(m, "Scene").def(nb::init<>()).def_rw("h", &PyRprScene::h);\
	nb::class_<PyRprCamera>(m, "Camera").def(nb::init<>()).def_rw("h", &PyRprCamera::h);\
	nb::class_<PyRprShape>(m, "Shape").def(nb::init<>()).def_rw("h", &PyRprShape::h);\
	nb::class_<PyRprLight>(m, "Light").def(nb::init<>()).def_rw("h", &PyRprLight::h);\
	nb::class_<PyRprFramebuffer>(m, "Framebuffer").def(nb::init<>()).def_rw("h", &PyRprFramebuffer::h);\
	nb::class_<PyRprContext>(m, "Context").def(nb::init<>()).def_rw("h", &PyRprContext::h);\
	nb::class_<PyRprImage>(m, "Image").def(nb::init<>()).def_rw("h", &PyRprImage::h);\
	nb::class_<PyRprBuffer>(m, "Buffer").def(nb::init<>()).def_rw("h", &PyRprBuffer::h);\
	nb::class_<PyRprHeteroVolume>(m, "HeteroVolume").def(nb::init<>()).def_rw("h", &PyRprHeteroVolume::h);\
	nb::class_<PyRprGrid>(m, "Grid").def(nb::init<>()).def_rw("h", &PyRprGrid::h);\
	nb::class_<PyRprCurve>(m, "Curve").def(nb::init<>()).def_rw("h", &PyRprCurve::h);\
	nb::class_<PyRprMaterialSystem>(m, "MaterialSystem").def(nb::init<>()).def_rw("h", &PyRprMaterialSystem::h);\
	nb::class_<PyRprMaterialNode>(m, "MaterialNode").def(nb::init<>()).def_rw("h", &PyRprMaterialNode::h);\
	nb::class_<PyRprPostEffect>(m, "PostEffect").def(nb::init<>()).def_rw("h", &PyRprPostEffect::h);\
	nb::class_<PyRprComposite>(m, "Composite").def(nb::init<>()).def_rw("h", &PyRprComposite::h);\
	nb::class_<PyRprLut>(m, "Lut").def(nb::init<>()).def_rw("h", &PyRprLut::h);\
	m.def("Int64ToPvoid", []( int64_t i ){return (void*)i;});\
	nb::class_<PyMalloc>(m, "PyMalloc").def(nb::init< nb::ndarray<>  >()).def(nb::init< const std::string&  >()).def("GetMem", &PyMalloc::GetMem).def("Free", &PyMalloc::Free);\




#define RPRPY_ADDITIONAL_DEFINES_rprs\
\
	nb::class_<PyRprsAnimation>(m, "Animation")\
	.def(nb::init<>())\
	.def_rw("structSize", &PyRprsAnimation::structSize)\
	.def_rw("groupName", &PyRprsAnimation::groupName)\
	.def_rw("movementType", &PyRprsAnimation::movementType)\
	.def_rw("interpolationType", &PyRprsAnimation::interpolationType)\
	.def_rw("nbTimeKeys", &PyRprsAnimation::nbTimeKeys)\
	.def_rw("nbTransformValues", &PyRprsAnimation::nbTransformValues)\
	.def_rw("timeKeys", &PyRprsAnimation::timeKeys)\
	.def_rw("transformValues", &PyRprsAnimation::transformValues);\
\
	m.def("AddAnimation", []( PyRprsAnimation* anim )\
	{\
		rprs_animation animInput;\
		animInput.structSize = anim->structSize;\
		animInput.groupName = (char*)anim->groupName.c_str();\
		animInput.movementType = anim->movementType;\
		animInput.interpolationType = anim->interpolationType;\
		animInput.nbTimeKeys = anim->nbTimeKeys;\
		animInput.nbTransformValues = anim->nbTransformValues;\
		animInput.timeKeys = &(anim->timeKeys[0]);\
		animInput.transformValues = &(anim->transformValues[0]);\
		PYRPR_CHECK_ERROR( rprsAddAnimation(  &animInput   )  );\
		return ret;\
	});\
	m.def("AddAnimationEx", []( PyRprsContext* rprsCtx,  PyRprsAnimation* anim )\
	{\
		rprs_animation animInput;\
		animInput.structSize = anim->structSize;\
		animInput.groupName = (char*)anim->groupName.c_str();\
		animInput.movementType = anim->movementType;\
		animInput.interpolationType = anim->interpolationType;\
		animInput.nbTimeKeys = anim->nbTimeKeys;\
		animInput.nbTransformValues = anim->nbTransformValues;\
		animInput.timeKeys = &(anim->timeKeys[0]);\
		animInput.transformValues = &(anim->transformValues[0]);\
		PYRPR_CHECK_ERROR( rprsAddAnimationEx(  rprsCtx->h,  &animInput   )  );\
		return ret;\
	});\
\
	nb::class_<PyRprsContext>(m, "Context").def(nb::init<>()).def_rw("h", &PyRprsContext::h);\
\






#define RPRPY_ADDITIONAL_DEFINES_rprgltf\
\
	nb::class_<PyRprgltfAnimation>(m, "Animation")\
	.def(nb::init<>())\
	.def_rw("structSize", &PyRprgltfAnimation::structSize)\
	.def_rw("groupName", &PyRprgltfAnimation::groupName)\
	.def_rw("movementType", &PyRprgltfAnimation::movementType)\
	.def_rw("interpolationType", &PyRprgltfAnimation::interpolationType)\
	.def_rw("nbTimeKeys", &PyRprgltfAnimation::nbTimeKeys)\
	.def_rw("nbTransformValues", &PyRprgltfAnimation::nbTransformValues)\
	.def_rw("timeKeys", &PyRprgltfAnimation::timeKeys)\
	.def_rw("transformValues", &PyRprgltfAnimation::transformValues);\
\
	m.def("AddAnimation", []( PyRprgltfAnimation* anim )\
	{\
		rprgltf_animation animInput;\
		animInput.structSize = anim->structSize;\
		animInput.groupName = (char*)anim->groupName.c_str();\
		animInput.movementType = anim->movementType;\
		animInput.interpolationType = anim->interpolationType;\
		animInput.nbTimeKeys = anim->nbTimeKeys;\
		animInput.nbTransformValues = anim->nbTransformValues;\
		animInput.timeKeys = &(anim->timeKeys[0]);\
		animInput.transformValues = &(anim->transformValues[0]);\
		PYRPR_CHECK_ERROR( rprGLTF_AddAnimation(  &animInput   )  );\
		return ret;\
	});\
\







