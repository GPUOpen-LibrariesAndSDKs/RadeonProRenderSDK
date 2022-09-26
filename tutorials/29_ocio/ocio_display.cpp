
#include "ocio_display.h"

#include <memory>

#if ( USE_OCIO == 1 )
#include <OpenColorIO/OpenColorIO.h>
#include <OpenColorIO/OpenColorTypes.h>
namespace OCIO = OCIO_NAMESPACE;
#endif

#include <Math/float3.h>
#include <Math/mathutils.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../3rdParty/stbi/stbi.h"

unsigned char FloatToByte(float f)
{
	return (unsigned char)RadeonProRender::clamp( (int)(f*255.0f) , (int)0, (int)255);
}


rpr_status OcioDisplay::Display(
	rpr_framebuffer framebuffer, 
	const std::string& ocioFile, 
	const std::string& src,
	
	const std::string& display, 
	const std::string& view, 

	float exposure, 
	float gamma ,
	const std::string& file_path,

	bool useOCIO
	)
{
	rpr_status status = RPR_SUCCESS;

	rpr_framebuffer_desc fbInfoDesc;
	status = rprFrameBufferGetInfo(framebuffer,RPR_FRAMEBUFFER_DESC,sizeof(fbInfoDesc),&fbInfoDesc,0); 
	if ( status != RPR_SUCCESS ) 
		return status;

	const int64_t pxlCount = fbInfoDesc.fb_height * fbInfoDesc.fb_width;
	const int fltCount = pxlCount * 4;
	auto frame_buffer_data = std::make_unique<float[]>(fltCount);
	status = rprFrameBufferGetInfo(framebuffer, RPR_FRAMEBUFFER_DATA, fltCount*4 , frame_buffer_data.get() , NULL ); 
	if ( status != RPR_SUCCESS ) 
		return status;

	const int channelCountLDR = 3;
	auto framebufferLDR = std::make_unique<unsigned char[]>(pxlCount * channelCountLDR);


	#if ( USE_OCIO == 1 )

	OCIO::ConstProcessorRcPtr processor = nullptr;
	OCIO::ConstCPUProcessorRcPtr cpuProcessor = nullptr;;
	OCIO::ConstConfigRcPtr config = nullptr;

	// Create a basic Display View Transform.
	try
	{
		config = OCIO::Config::CreateFromFile( ocioFile.c_str() );

		OCIO::DisplayViewTransformRcPtr transform = OCIO::DisplayViewTransform::Create();
		transform->setSrc( src.c_str() );
		transform->setDisplay( display.c_str() );
		transform->setView( view.c_str() );

		OCIO::LegacyViewingPipelineRcPtr vp = OCIO::LegacyViewingPipeline::Create();
		vp->setDisplayViewTransform(transform);
		vp->setLooksOverrideEnabled(true);
		vp->setLooksOverride("");

		// exposure
		{
			double gain = exposure;
			const double slope4f[] = { gain, gain, gain, gain };
			double m44[16];
			double offset4[4];
			OCIO::MatrixTransform::Scale(m44, offset4, slope4f);
			OCIO::MatrixTransformRcPtr mtx =  OCIO::MatrixTransform::Create();
			mtx->setMatrix(m44);
			mtx->setOffset(offset4);
			vp->setLinearCC(mtx);
		}

		// Post-display transform gamma
		if ( gamma != 0.0 )
		{
			double exponent = 1.0/std::max(1e-6, (double)gamma);
			const double exponent4f[4] = { exponent, exponent, exponent, exponent };
			OCIO::ExponentTransformRcPtr expTransform =  OCIO::ExponentTransform::Create();
			expTransform->setValue(exponent4f);
			vp->setDisplayCC(expTransform);
		}
				
		processor = vp->getProcessor(config);
		cpuProcessor = processor->getDefaultCPUProcessor();

	}
	catch( std::exception& e )
	{
		const char* what = e.what();
		processor = nullptr;
		cpuProcessor = nullptr;
		config = nullptr;
		std::cout<<"Error during OCIO config: "<< what << std::endl;
		return RPR_ERROR_INTERNAL_ERROR;
	}

	#endif

	// run the post process on each pixels
	for(int64_t i=0; i<pxlCount; i++) // <-- can be multithreaded for perf improve
	{
		RadeonProRender::float3 rgb = RadeonProRender::float3(
			frame_buffer_data[i*4+0],
			frame_buffer_data[i*4+1],
			frame_buffer_data[i*4+2]);

		
		#if ( USE_OCIO == 1 )
		if ( useOCIO ) // if using OCIO processor
		{
			cpuProcessor->applyRGB(&(rgb.x));
		}
		else
		#endif
		{
			rgb.x = powf(rgb.x * exposure, 1.0f/gamma );
			rgb.y = powf(rgb.y * exposure, 1.0f/gamma );
			rgb.z = powf(rgb.z * exposure, 1.0f/gamma );
		}
		
		framebufferLDR[i*3+0] = FloatToByte(rgb[0]);
		framebufferLDR[i*3+1] = FloatToByte(rgb[1]);
		framebufferLDR[i*3+2] = FloatToByte(rgb[2]);
	}

	// export to png file.
	stbi_write_png(
		file_path.c_str(), 
		fbInfoDesc.fb_width,
		fbInfoDesc.fb_height,
		channelCountLDR,
		framebufferLDR.get(),
		fbInfoDesc.fb_width*channelCountLDR
		);

	return RPR_SUCCESS;
}
