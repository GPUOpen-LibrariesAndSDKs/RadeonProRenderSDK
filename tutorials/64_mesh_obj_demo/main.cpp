/*****************************************************************************\
*
*  Module Name    mesh_obj_demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/


//
//
// Project that import and rendering any OBJ file with Radeon ProRender.
//
//


#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h"//For Export
#include "../common/common.h"

#include "../common/picojson.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../common/tiny_obj_loader.h"

#include <cassert>
#include <iostream>
#include <map>

// Hold objects from different helper functions which are cleaned at end of main
std::vector<void*> garbageCollector;

std::map<std::string, rpr_creation_flags> creationModesMap = {
  {"gpu0", RPR_CREATION_FLAGS_ENABLE_GPU0},
  {"gpu1", RPR_CREATION_FLAGS_ENABLE_GPU1},
  {"gpu2", RPR_CREATION_FLAGS_ENABLE_GPU2},
  {"gpu3", RPR_CREATION_FLAGS_ENABLE_GPU3},
  {"gpu4", RPR_CREATION_FLAGS_ENABLE_GPU4},
  {"gpu5", RPR_CREATION_FLAGS_ENABLE_GPU5},
  {"gpu6", RPR_CREATION_FLAGS_ENABLE_GPU6},
  {"gpu7", RPR_CREATION_FLAGS_ENABLE_GPU7},
  {"gpu8", RPR_CREATION_FLAGS_ENABLE_GPU8},
  {"gpu9", RPR_CREATION_FLAGS_ENABLE_GPU9},
  {"gpu10", RPR_CREATION_FLAGS_ENABLE_GPU10},
  {"gpu11", RPR_CREATION_FLAGS_ENABLE_GPU11},
};

std::map<std::string, rpr_render_mode> renderModesMap = {
{ "gi", RPR_RENDER_MODE_GLOBAL_ILLUMINATION             }  ,
{ "di", RPR_RENDER_MODE_DIRECT_ILLUMINATION             }  ,
{ "di_no_shadow", RPR_RENDER_MODE_DIRECT_ILLUMINATION_NO_SHADOW   }  ,
{ "wire", RPR_RENDER_MODE_WIREFRAME                       }  ,
{ "mi", RPR_RENDER_MODE_MATERIAL_INDEX                  }  ,
{ "pos", RPR_RENDER_MODE_POSITION                        }  ,
{ "normal", RPR_RENDER_MODE_NORMAL                          }  ,
{ "tex", RPR_RENDER_MODE_TEXCOORD                        }  ,
{ "ao", RPR_RENDER_MODE_AMBIENT_OCCLUSION               }  ,
{ "diff", RPR_RENDER_MODE_DIFFUSE                         }  ,
};

std::map<std::string, rpr_camera_mode> cameraModesMap = {
	{"perspective", RPR_CAMERA_MODE_PERSPECTIVE},
	{"orthogrphics", RPR_CAMERA_MODE_ORTHOGRAPHIC},
	{"longlat360", RPR_CAMERA_MODE_LATITUDE_LONGITUDE_360},
	{"longlatstereo", RPR_CAMERA_MODE_LATITUDE_LONGITUDE_STEREO},
	{"cubemap", RPR_CAMERA_MODE_CUBEMAP},
	{"cubemapstereo", RPR_CAMERA_MODE_CUBEMAP_STEREO},
	{"fisheye", RPR_CAMERA_MODE_FISHEYE}
};

template<typename T>
inline
void fill(const picojson::object& v, const char* key, T& dst)
{
	auto c = v.find(key);
	if (c != v.end())
	{
		const picojson::value& d = c->second;
		dst = static_cast<T>(d.get<T>());
	}
}

template<>
inline
void fill(const picojson::object& v, const char* key, RadeonProRender::float3& dst)
{
	auto c = v.find(key);
	if (c != v.end())
	{
		int j = 0;
		const picojson::value& d = c->second;
		const picojson::array &a = d.get<picojson::array>();
		for (picojson::array::const_iterator i = a.begin(); i != a.end(); ++i) {
			dst[j++] = std::stof(i->to_str());
		}
		
	}
}

template<>
inline
void fill(const picojson::object& v, const char* key, RadeonProRender::float4& dst)
{
	auto c = v.find(key);
	if (c != v.end())
	{
		int j = 0;
		const picojson::value& d = c->second;
		const picojson::array &a = d.get<picojson::array>();
		for (picojson::array::const_iterator i = a.begin(); i != a.end(); ++i) {
			dst[j++] = std::stof(i->to_str());
		}
		
	}
}

template<>
inline
void fill(const picojson::object& v, const char* key, float& dst)
{
	auto c = v.find(key);
	if (c != v.end())
	{
		const picojson::value& d = c->second;
		dst = static_cast<double>(d.get<double>());
	}
}

struct Context
{
	rpr_context context;
	rpr_scene scene;
	rpr_framebuffer fb;
	rpr_framebuffer fb1;
	uint32_t max_iterations = 64;
	std::string outFileName = "64.png";
};

struct ContextSettings
{
	rpr_render_mode renderMode = RPR_RENDER_MODE_GLOBAL_ILLUMINATION;
	rpr_uint recursion = 10;
	rpr_uint width = 1280;
	rpr_uint height = 720;
	rpr_uint iterations = 64;
	rpr_uint batchSize = 32;
	std::string outImgFile = "test.png";
	std::string outJsonFile = "output.json";
	rpr_uint gui = 0;
	rpr_creation_flags creationFlags = RPR_CREATION_FLAGS_ENABLE_GPU0;
};

struct CameraSettings
{
	RadeonProRender::float3 position = { 0.0, -1.0, 21.0 };
	RadeonProRender::float3 aimed = { 0.0f, 0.0f, 0.0f };
	RadeonProRender::float3 up = { 0.0f, 1.0f, 0.0f };
	rpr_float focalLength = 75.0f;
	RadeonProRender::float3 translation = { 0.0f, 0.0f, 0.0f };
	RadeonProRender::float4 rotation = { 0.0f, 1.0f, 0.0f, 3.14159f };
	rpr_camera_mode cameraMode = RPR_CAMERA_MODE_PERSPECTIVE;
};

struct LightSettings
{
   std::string type = "point";
   RadeonProRender::float3 translation = { 0.0f, 1.0f, 0.0f };
   RadeonProRender::float4 rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
   RadeonProRender::float3 radiantPower = { 55.0f, 45.0f, 45.0f };
   rpr_float intensity = 10.0f;
};

struct EnvLightSettings
{
	//Currently we only specify image path but migh add multiple settings here
	std::string path = "../../Resources/Textures/envLightImage.exr";
};

struct ShapeSettings
{
	std::string name = "cornelBox";
	std::string path = "../../Resources/Meshes/cornellBox.obj";
	RadeonProRender::float3 translation = { 0.0f, 0.0f, 0.0f };
	RadeonProRender::float4 rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	RadeonProRender::float3 scale = {0.0f, 0.0f, 0.0f};
};

struct Configuration
{
	ContextSettings contextSettings;
	CameraSettings cameraSettings;
	std::vector<LightSettings> lightSettings;
	std::vector<ShapeSettings> shapeSettings;
	std::vector<EnvLightSettings> envLightSettings;
};

const ContextSettings parseContextSettings(const picojson::value& config)
{
	ContextSettings settings;

	auto context = config.get<picojson::object>();

	std::string renderMode;
	fill(context, "rendermode", renderMode);
	settings.renderMode = renderModesMap[renderMode];

	std::string recursion;
	fill(context, "recursion", recursion);
	settings.recursion = std::stoi(recursion);

	std::string width;
	fill(context, "width", width);
	settings.width = std::stoi(width);

	std::string height;
	fill(context, "height", height);
	settings.height = std::stoi(height);

	std::string iterations;
	fill(context, "iterations", iterations);
	settings.iterations = std::stoi(iterations);

	std::string batchSize;
	fill(context, "batchsize", batchSize);
	settings.batchSize = std::stoi(batchSize);


	fill(context, "output", settings.outImgFile);
	fill(context, "output.json", settings.outJsonFile);
	
	std::string gui;
	fill(context, "gui", gui);
	settings.gui = std::stoi(gui);

	std::string cpu;
	std::string gpu;

	auto c = context.find("device")->second.get<picojson::object>();
   
	fill(c, "gpu0", gpu);
	fill(c, "cpu", cpu);
   
	if (std::stoi(gpu))
	{
		settings.creationFlags = creationModesMap[c.find("gpu0")->first];
	}

	if (std::stoi(cpu))
	{
		settings.creationFlags |= RPR_CREATION_FLAGS_ENABLE_CPU;
	}


	#if defined(USING_NORTHSTAR) && defined(__APPLE__) 
	settings.creationFlags |= RPR_CREATION_FLAGS_ENABLE_METAL; // by default always enable Metal for MacOS
	#endif

	return settings;
}

const CameraSettings parseCameraSettings(const picojson::value& config)
{
	CameraSettings settings;

	auto camera = config.get<picojson::object>();
	
	fill(camera, "position", settings.position);
	fill(camera, "aimed", settings.aimed);
	fill(camera, "up", settings.up);
	fill(camera, "focal_length", settings.focalLength);
	fill(camera, "translation", settings.translation);
	fill(camera, "rotation", settings.rotation);

	std::string cameraMode;
	fill(camera, "type", cameraMode);
	settings.cameraMode = cameraModesMap[cameraMode];

	return settings;
}

const LightSettings parseLightSettings(const picojson::value& config)
{
	LightSettings settings;

	auto light = config.get<picojson::object>();

	fill(light, "type", settings.type);
	fill(light, "translation", settings.translation);
	fill(light, "rotation", settings.rotation);
	fill(light, "radiant_power", settings.radiantPower);
	fill(light, "intensity", settings.intensity);

	return settings;
}

const EnvLightSettings parseEnvLightSettings(const picojson::value& config)
{
	EnvLightSettings e;
	std::string path;

	auto ibl = config.get<picojson::object>();
	fill(ibl, "path", path);
	e.path = path;

	return e;
}

const ShapeSettings parseShapeSettings(const picojson::value& config)
{
	ShapeSettings settings;

	auto obj = config.get<picojson::object>();

	fill(obj, "name", settings.name);
	fill(obj, "geomObjFile", settings.path);
	fill(obj, "translation", settings.translation);
	fill(obj, "rotation", settings.rotation);
	fill(obj, "scale", settings.scale);

	return settings;
}

const Configuration loadConfigFile(const char* filepath)
{
	std::ifstream i(filepath, std::ios::binary);
	if (!i.is_open())
	{
		std::cout << "Config file " << filepath << " not found." << '\n';
		std::exit(EXIT_FAILURE);
	}

	std::size_t sizeInByte = 0;
	{
		std::streampos begin = i.tellg();
		i.seekg(0, std::ios::end);
		std::streampos end = i.tellg();
		sizeInByte = end - begin;
	}

	if (sizeInByte == 0)
	{
		std::cout << "Config file" << filepath << "is empty." << '\n';
		std::cout << "Continue with default configuration .... \n";
	}

	i.clear();
	i.seekg(0, std::ios::beg);

	std::vector<char> json;
	json.resize(sizeInByte);
	i.read(json.data(), sizeInByte);
	i.close();

	picojson::value v;
	std::string err;
	const char* json_end = picojson::parse(v, json.data(), json.data() + json.size(), &err);
   
	Configuration config;
	bool bContext = false;
	bool bLight = false;
	bool bCamera = false;
	bool bIbl = false;
	bool bShape = false;

	try
	{
		if (err.empty())
		{
			const picojson::object& o = v.get<picojson::object>();

			for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i)
			{
				if (i->first.find("context") != std::string::npos)
				{
					config.contextSettings = parseContextSettings(i->second);
					bContext = true;
				}
				else if (i->first.find("obj") != std::string::npos)
				{
					config.shapeSettings.push_back(parseShapeSettings(i->second));
					bShape = true;
				}
				else if (i->first.find("light") != std::string::npos)
				{
					config.lightSettings.push_back(parseLightSettings(i->second));
					bLight = true;
				}
				else if (i->first.find("camera") != std::string::npos)
				{
					config.cameraSettings = std::move(parseCameraSettings(i->second));
					bCamera = true;
				}
				if (i->first.find("ibl") != std::string::npos)
				{
					config.envLightSettings.push_back(parseEnvLightSettings(i->second));
					bIbl = true;
				}
			}
		}
	}
	catch (std::exception&)
	{
		std::cout << "\nSyntax error with config file!!!\n";
		std::exit(EXIT_FAILURE);
	}

	if (!bContext)
	{
		std::cout << "\nNo context defined in config, Initializing default context : \n";
		std::cout << "Creation Flags     : " << config.contextSettings.creationFlags << '\n';
		std::cout << "Max Iterations     : " << config.contextSettings.iterations << '\n';
		std::cout << "GUI Mode           : " << config.contextSettings.gui << '\n';
		std::cout << "Framebuffer Height : " << config.contextSettings.height << '\n';
		std::cout << "Framebuffer Width  : " << config.contextSettings.width << '\n';
		std::cout << "Rendermode         : " << config.contextSettings.renderMode << '\n';
		std::cout << "Recursion          : " << config.contextSettings.recursion << '\n';
		std::cout << "Batchsize          : " << config.contextSettings.batchSize << '\n';
		std::cout << "outImageFile       : " << config.contextSettings.outImgFile << "\n\n";
	}

	if (!bLight && !bIbl)
	{
		EnvLightSettings settings;
		std::cout << "\nNo lights are defined config, Initializing default Image based lighting :\n";
		std::cout << "Image based light : " << settings.path << '\n';
		config.envLightSettings.push_back(settings);
	}

	if (!bCamera)
	{
		std::cout << "\nNo camera defined in config, Initializing default camera : \n";
		std::cout << "Aimed       :" << config.cameraSettings.aimed << '\n';
		std::cout << "Position    :" << config.cameraSettings.position << '\n';
		std::cout << "Rotation    :" << config.cameraSettings.rotation << '\n';
		std::cout << "Translation :" << config.cameraSettings.translation << '\n';
		std::cout << "Up          :" << config.cameraSettings.up << '\n';
		std::cout << "Focal Length:" << config.cameraSettings.focalLength << '\n';
		std::cout << "Camera Mode :" << config.cameraSettings.cameraMode << "\n\n";
	}

	if (!bShape)
	{
		ShapeSettings settings;
		std::cout << "\nNo shapes defined in config, Initializing default shape : \n";
		std::cout << "Name        : " << settings.name << '\n';
		std::cout << "Path        :" << settings.path << '\n';
		std::cout << "Rotation    : " << settings.rotation << '\n';
		std::cout << "Translation : " << settings.translation << '\n';
		std::cout << "Scale       : " << settings.scale << "\n\n";

		settings.translation = RadeonProRender::float3{0.0f, -3.0f, 0.0f};
		settings.scale = RadeonProRender::float3{1.0f, 1.0f, 1.0f};
		config.shapeSettings.push_back(settings);
	}

	return config;
}

Context createContext(const ContextSettings settings)
{
	Context outContext;

	rpr_int status = RPR_SUCCESS;

	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME);
	CHECK_NE(tahoePluginID, -1);

	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using single GPU 
	CHECK(rprCreateContext(RPR_API_VERSION, plugins, pluginCount, settings.creationFlags, NULL, NULL, &outContext.context));

	CHECK(rprContextSetActivePlugin(outContext.context, plugins[0]));

	if (status != RPR_SUCCESS)
	{
		std::cout << "RPR Context creation failed.\n";
		std::exit(EXIT_FAILURE);
	}

	CHECK(rprContextCreateScene(outContext.context, &outContext.scene));

	CHECK(rprContextSetScene(outContext.context, outContext.scene));

	rpr_framebuffer_desc desc = { settings.width , settings.height };

	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
  
	CHECK(rprContextCreateFrameBuffer(outContext.context, fmt, &desc, &outContext.fb));

	CHECK(rprFrameBufferClear(outContext.fb));

	CHECK(rprContextCreateFrameBuffer(outContext.context, fmt, &desc, &outContext.fb1));

	CHECK(rprFrameBufferClear(outContext.fb1));

	CHECK(rprContextSetAOV(outContext.context, RPR_AOV_COLOR, outContext.fb));

	CHECK(rprContextSetParameterByKey1u(outContext.context, RPR_CONTEXT_RENDER_MODE, settings.renderMode));

	outContext.max_iterations = settings.iterations;
	outContext.outFileName = settings.outImgFile;
	return outContext;
}


void createAndAttachCamera(rpr_context& context, rpr_scene& scene, const CameraSettings& settings)
{
	// ToDo: add code to create other types of cameras
	rpr_camera camera = nullptr;

	if(settings.cameraMode & RPR_CAMERA_MODE_PERSPECTIVE)
	{
		CHECK(rprContextCreateCamera(context, &camera));

		CHECK(rprCameraLookAt(camera, settings.position.x, settings.position.y, settings.position.z,
			settings.aimed.x, settings.aimed.y, settings.aimed.z,
			settings.up.x, settings.up.y, settings.up.z));

		CHECK(rprCameraSetFocalLength(camera, settings.focalLength));

		CHECK(rprCameraSetMode(camera, settings.cameraMode));

		CHECK(rprSceneSetCamera(scene, camera));
	}

	garbageCollector.push_back(camera);
}

void createAndAttachLight(rpr_context& context, rpr_scene& scene, const LightSettings& settings)
{
	// ToDo: add code to create other types of lights
	rpr_light light = nullptr;

	if(settings.type == "point")
	{
		CHECK(rprContextCreatePointLight(context, &light));

		RadeonProRender::matrix lightm = RadeonProRender::translation(settings.translation);

		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));

		// Radiant power
		CHECK(rprPointLightSetRadiantPower3f(light, settings.radiantPower.x, settings.radiantPower.x, settings.radiantPower.x));

		CHECK(rprSceneAttachLight(scene, light));
	}

	garbageCollector.push_back(light);
}

void createAndAttachEnvironmentLight(rpr_context& context, rpr_scene& scene, const EnvLightSettings& settings)
{
	if (settings.path.empty())
	{
		return; //No image based lighting
	}

	rpr_int status = RPR_SUCCESS;
	rpr_light light = nullptr;
	rpr_image img = nullptr;
	{
		CHECK(rprContextCreateEnvironmentLight(context, &light));

		CHECK(rprContextCreateImageFromFile(context, settings.path.c_str(), &img));
		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error Ibl file : " << settings.path << " not found.\n";
			std::exit(EXIT_FAILURE);
		}

		// Set an image for the light to take the radiance values from
		CHECK(rprEnvironmentLightSetImage(light, img));

		CHECK(rprSceneAttachLight(scene, light));
	}

	garbageCollector.push_back(light);
	garbageCollector.push_back(img);
}

void loadTexture(rpr_context& context, rpr_material_node mat, std::string texture)
{
		rpr_image img;
		rpr_int status;

		status = rprContextCreateImageFromFile(context, texture.c_str(), &img);

		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error in loadTexture() : " << texture << " not found.\n";
			std::exit(EXIT_FAILURE);
		}

		CHECK(status);
		
		CHECK(rprMaterialNodeSetInputImageDataByKey(mat, RPR_MATERIAL_INPUT_DATA, img));

		garbageCollector.push_back(img);
}

void loadAndAttachShapes(rpr_context& context, rpr_scene& scene, rpr_material_system matsys, const ShapeSettings& settings)
{
	// ToDo : parse more material options
	struct Material
	{
		RadeonProRender::float3 dColor{ 0.0f };
		RadeonProRender::float3 aColor{ 0.0f }; //Not used currently
		RadeonProRender::float3 sColor{ 0.0f };
		RadeonProRender::float3 eColor{ 0.0f };
		RadeonProRender::float3 absorptionColor{ 0.0f };
		rpr_float ior = 0.0f; //Ni
		rpr_float roughness = 0.0f;
		rpr_float shininess = 0.0f;// Ns specular weight
		rpr_float d = 1.0f; //dissolve default : opaque
		rpr_int illum = 2; //default illumination model
		std::string dTexture;
		std::string bTexture;
		std::string alphaTexture;
		std::string name;
	};

	const auto lerp = [](const float a, const float b, const float t)->float {

		return (t * b) + (1 - t) * a;
	};

	const auto inverseLerp = [](const float a, const float b, const float value)->float {
		return (value - a) / (b - a);
	};

	const auto isPresent = [](const RadeonProRender::float3& color)->bool {
		return (color.x != 0.0f) || (color.y != 0.0f) || (color.z != 0.0f);
	};

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	std::string warning;
	
	std::string mtlBaseDir = settings.path.substr(0, settings.path.rfind("/"));

	if (mtlBaseDir.empty()) {
		mtlBaseDir = ".";
	}

	mtlBaseDir += "/";

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &err, settings.path.c_str(), mtlBaseDir.c_str());

	if (!warning.empty()) {
		std::cout << "OBJ Loader WARN : " << warning << '\n';
	}

	if (!err.empty()) {
		std::cout << "OBJ Loader ERROR : " << err << '\n';
	}

	if (!ret) {
		std::cout << "Failed to load obj file\n";
		std::exit(EXIT_FAILURE);
	}

	//Parse materials
	auto convert = [](const tinyobj::real_t c[3])->RadeonProRender::float3 {
		return RadeonProRender::float3{ c[0], c[1], c[2] };
	};

	// Map holds particular material of material_id
	uint32_t matId = 0;
	std::map<uint32_t, Material> matMap;
	for (const auto& mat : materials)
	{
		Material m;

		m.aColor = convert(mat.ambient);
		m.dColor = convert(mat.diffuse);
		m.sColor = convert(mat.specular);
		m.eColor = convert(mat.emission);
		m.absorptionColor = convert(mat.transmittance);
		m.dTexture = mat.diffuse_texname;
		m.bTexture = mat.bump_texname;
		m.alphaTexture = mat.alpha_texname;
		m.ior = mat.ior;
		m.d = mat.dissolve;
		m.illum = mat.illum;
		m.roughness = mat.roughness;
		m.shininess = mat.shininess;
		m.name = mat.name;
		matMap[matId++] = m;
	}

	// Load geometry
	const rpr_int vertexStride = 3 * sizeof(tinyobj::real_t);
	const rpr_int normalStride = 3 * sizeof(tinyobj::real_t);
	const rpr_int texCoordStride = 2 * sizeof(tinyobj::real_t);;

	for (size_t i = 0; i < shapes.size(); ++i)
	{
		std::vector<rpr_int>faceVert(shapes[i].mesh.num_face_vertices.begin(), shapes[i].mesh.num_face_vertices.end());
		std::map<uint32_t, std::vector<uint32_t>> faceMaterialMap;     //Map holds face indices for particular material_id
		std::vector<uint32_t> vIndices;
		std::vector<uint32_t> nIndices;
		std::vector<uint32_t> tIndices;

		bool isValidTex = true;
		bool isValidNorm = true;

		for (size_t j = 0; j < shapes[i].mesh.indices.size() / 3; j++)
		{
			tinyobj::index_t idx0 = shapes[i].mesh.indices[3 * j + 0];
			tinyobj::index_t idx1 = shapes[i].mesh.indices[3 * j + 1];
			tinyobj::index_t idx2 = shapes[i].mesh.indices[3 * j + 2];

			vIndices.push_back(idx0.vertex_index); vIndices.push_back(idx1.vertex_index); vIndices.push_back(idx2.vertex_index);

			if (idx0.normal_index < 0 || idx1.normal_index < 0 || idx2.normal_index < 0)
			{
				isValidNorm = false;
			}
			else
			{
				nIndices.push_back(idx0.normal_index); nIndices.push_back(idx1.normal_index); nIndices.push_back(idx2.normal_index);
			}

			if (idx0.texcoord_index < 0 || idx1.texcoord_index < 0 || idx2.texcoord_index < 0)
			{
				isValidTex = false;
			}
			else
			{
				tIndices.push_back(idx0.texcoord_index); tIndices.push_back(idx1.texcoord_index); tIndices.push_back(idx2.texcoord_index);
			}
		}

		//Fill material corresponding faces in map
		for (size_t face = 0; face < shapes[i].mesh.num_face_vertices.size(); face++)
		{
			const auto matId = shapes[i].mesh.material_ids[face];
			faceMaterialMap[matId].push_back(face);
		}

		rpr_shape t_shape = nullptr;
		{
			CHECK(rprContextCreateMesh(context,
				(rpr_float const*)attrib.vertices.data(),
				(size_t)attrib.vertices.size() / 3,
				vertexStride,
				(isValidNorm) ? (rpr_float const*)attrib.normals.data() : nullptr,
				(isValidNorm) ? (size_t)attrib.normals.size() / 3 : 0,
				(isValidNorm) ? normalStride : 0,
				(isValidTex) ? (rpr_float const*)attrib.texcoords.data() : nullptr,
				(isValidTex) ? (size_t)attrib.texcoords.size() / 2 : 0,
				(isValidTex) ? texCoordStride : 0,
				(rpr_int const*)vIndices.data(),
				sizeof(int),
				(isValidNorm) ? (rpr_int const*)nIndices.data() : nullptr,
				(isValidNorm) ? sizeof(int) : 0,
				(isValidTex) ? (rpr_int const*)tIndices.data() : nullptr,
				(isValidTex) ? sizeof(int) : 0,
				(rpr_int const*)faceVert.data(),
				(size_t)faceVert.size(),
				&t_shape));

			CHECK(rprSceneAttachShape(scene, t_shape));

			RadeonProRender::matrix m = RadeonProRender::translation(settings.translation) * RadeonProRender::scale(settings.scale);

			CHECK(rprShapeSetTransform(t_shape, RPR_TRUE, &m.m00));
		}

		// Avoid applying material to per face
		bool bAvoidFaceMat = (faceMaterialMap.size() == 1) ? true : false;

		//Apply materials per face
		for (const auto& fm : faceMaterialMap)
		{
			const Material& m = matMap[fm.first];
			const auto& faceIndices = fm.second;

			rpr_material_node t_uber = nullptr;
				
			CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_UBERV2, &t_uber));
			CHECK(bAvoidFaceMat ? rprShapeSetMaterial(t_shape, t_uber) : 
				  rprShapeSetMaterialFaces(t_shape, t_uber, (rpr_int const*)faceIndices.data(), faceIndices.size()));

			// ToDo : Add code to set other types of textures
			rpr_material_node t_diffuse = nullptr;
			if (!m.dTexture.empty())
			{
				const std::string texture = mtlBaseDir + "Textures/" + m.dTexture;

				rpr_material_node t_tex = nullptr;

				CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &t_diffuse));
				CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &t_tex));

				loadTexture(context, t_tex, texture);

				CHECK(rprMaterialNodeSetInputNByKey(t_diffuse, RPR_MATERIAL_INPUT_COLOR, t_tex));

				CHECK(bAvoidFaceMat ? rprShapeSetMaterial(t_shape, t_diffuse) :
					rprShapeSetMaterialFaces(t_shape, t_diffuse, (rpr_int const*)faceIndices.data(), faceIndices.size()));

				garbageCollector.push_back(t_tex);
				garbageCollector.push_back(t_diffuse);
			}

			if (!m.bTexture.empty())
			{
				const std::string texture = mtlBaseDir + "Textures/" + m.bTexture;
				rpr_material_node t_micro = nullptr;
				rpr_material_node t_tex = nullptr;
				rpr_material_node t_mat = nullptr;

				rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_MICROFACET, &t_micro);
				CHECK(rprMaterialNodeSetInputNByKey(t_micro, RPR_MATERIAL_INPUT_COLOR, t_diffuse));

				CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_BUMP_MAP, &t_mat));
				CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &t_tex));

				loadTexture(context, t_tex, texture);

				CHECK(rprMaterialNodeSetInputNByKey(t_mat, RPR_MATERIAL_INPUT_COLOR, t_tex));

				CHECK(rprMaterialNodeSetInputNByKey(t_micro, RPR_MATERIAL_INPUT_NORMAL, t_mat));

				CHECK(bAvoidFaceMat ? rprShapeSetMaterial(t_shape, t_micro) :
					rprShapeSetMaterialFaces(t_shape, t_micro, (rpr_int const*)faceIndices.data(), faceIndices.size()));

				garbageCollector.push_back(t_mat);
				garbageCollector.push_back(t_micro);
				garbageCollector.push_back(t_tex);
			}

			if (isPresent(m.eColor))
			{
				rpr_material_node t_emissive = nullptr;

				CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_EMISSIVE, &t_emissive));
				CHECK(rprMaterialNodeSetInputFByKey(t_emissive, RPR_MATERIAL_INPUT_COLOR, m.eColor.x, m.eColor.y, m.eColor.z, 1.f));
				CHECK(bAvoidFaceMat ? rprShapeSetMaterial(t_shape, t_emissive) : 
					  rprShapeSetMaterialFaces(t_shape, t_emissive, (rpr_int*)faceIndices.data(), faceIndices.size()));

				garbageCollector.push_back(t_emissive);
			}
			else
			{
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR, m.eColor.x, m.eColor.y, m.eColor.z, 1.f));
			}

			CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR, m.dColor.x, m.dColor.y, m.dColor.z, 1.f));

			if (m.illum == 5 || m.illum == 7) //Reflection and ray trace on
			{
				const rpr_float weight = inverseLerp(0.0f, 1000.0f, m.shininess);
				const float ior = (m.ior == 1.0f) ? m.ior * 3.0f : m.ior;

				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR, m.sColor.x, m.sColor.y, m.sColor.z, 1.f));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT, weight, weight, weight, weight));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR, ior, ior, ior, ior));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS, m.roughness, m.roughness, m.roughness, m.roughness));
			}

			if (m.illum == 7)//Refraction and ray trace on
			{
				const rpr_float weight = inverseLerp(0.0f, 1000.0f, m.shininess);

				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR, 1.0f - m.absorptionColor.x, 1.0f -  m.absorptionColor.y, 1.0f -  m.absorptionColor.z, 1.f));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT, weight, weight, weight, weight));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR, m.ior, m.ior, m.ior, m.ior));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS, m.roughness, m.roughness, m.roughness, m.roughness));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR, m.absorptionColor.x, m.absorptionColor.y, m.absorptionColor.z, 1.f));
				CHECK(rprMaterialNodeSetInputFByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE, 0.0f, 0.0f ,0.0f , 1.f));
				CHECK(rprMaterialNodeSetInputUByKey(t_uber, RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS, 1));
				CHECK(rprContextSetParameterByKey1u(context, RPR_CONTEXT_MAX_RECURSION, 10));
			}

			garbageCollector.push_back(t_uber);
		}//Face
		garbageCollector.push_back(t_shape);
	}//Shape
}

void printHelp()
{
	// ToDo : print config specs
	std::cout << "64_rpr_mesh_obj_demo.exe cfg.json [-h | --help]" << '\n';
	return;
}

// example: 64_mesh_obj_demo.exe ../64_mesh_obj_demo/cfg.json
int main(int argc , char* argv[])
{
#ifdef ENABLE_TRACING
	rprContextSetParameterByKey1u(0, RPR_CONTEXT_TRACING_ENABLED, 1);
#endif

	std::cout << "Radeon ProRender obj mesh rendering tutorial.\n";

	if (argc < 2 || ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)))
	{
		printHelp();
		return 0;
	}

	//Load config 
	const Configuration config = loadConfigFile(argv[1]);

	//Create global context holding rpr_context, rpr_scene and rpr_framebuffer
	Context contextSettings = createContext(config.contextSettings);

	rpr_context context = std::move(contextSettings.context);
	rpr_scene scene = std::move(contextSettings.scene);
	rpr_framebuffer frameBuffer = std::move(contextSettings.fb);

	std::cout << "\nContext Successfully create!.\n";

	// Create and attach camerato scene
	createAndAttachCamera(context, scene, config.cameraSettings);

	rpr_material_system matsys = nullptr;
	CHECK(rprContextCreateMaterialSystem(context, 0, &matsys));

	// Load models from config file and attach them to scene
	for (const auto& setting : config.shapeSettings)
	{
		loadAndAttachShapes(context, scene, matsys, setting);
	}

	// Create and attach light to scene
	for (const auto& setting : config.lightSettings)
	{
		createAndAttachLight(context, scene, setting);
	}
   
	// Create and attach environment light
	for (const auto& setting : config.envLightSettings)
	{
		createAndAttachEnvironmentLight(context, scene, setting);
	}

	rpr_post_effect postEffectGamma = 0;
	rpr_post_effect normalizationEff = 0;
	{
		CHECK(rprContextCreatePostEffect(context, RPR_POST_EFFECT_NORMALIZATION, &normalizationEff));
		CHECK(rprContextAttachPostEffect(context, normalizationEff));

		CHECK(rprContextCreatePostEffect(context, RPR_POST_EFFECT_GAMMA_CORRECTION, &postEffectGamma));
		CHECK(rprContextAttachPostEffect(context, postEffectGamma));

		//RPR_CONTEXT_DISPLAY_GAMMA
		CHECK(rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA, 2.2f));
	}

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,contextSettings.max_iterations));
	CHECK(rprContextRender(context));

	std::cout << "\nRendering finished.\n";

	// Save the result to file
	rpr_framebuffer frameBuffer1 = std::move(contextSettings.fb1);
	CHECK(rprContextResolveFrameBuffer(context, frameBuffer, frameBuffer1, false)); //resolve postprocess
	CHECK(rprFrameBufferSaveToFile(frameBuffer1, contextSettings.outFileName.c_str()));

	// Release the stuff we created
	rprObjectDelete( postEffectGamma ); postEffectGamma = nullptr;
	rprObjectDelete( normalizationEff ); normalizationEff = nullptr;
	CHECK(rprObjectDelete(scene)); scene = nullptr;
	CHECK(rprObjectDelete(frameBuffer)); frameBuffer = nullptr;
	CHECK(rprObjectDelete(frameBuffer1)); frameBuffer1 = nullptr;
	CHECK(rprObjectDelete(matsys)); matsys = nullptr;
	for (const auto& t : garbageCollector)
	{
		CHECK(rprObjectDelete(t));
	}
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context = nullptr; // Always delete the RPR Context in last.

	return 0;
}
