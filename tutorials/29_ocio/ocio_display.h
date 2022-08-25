#pragma once

// set it to 1 if OpenColorIO library is linked to this project.
#define USE_OCIO 0

#include "ocio_display.h"

#include <RadeonProRender.h>

#include <string>

class OcioDisplay
{

public:

	// export a resolved HDR framebuffer into a PNG.
	static rpr_status Display(
		rpr_framebuffer framebuffer, 

		const std::string& ocioFile, 
		const std::string& src, // color space of the framebuffer ( example: "ACES - ACEScg" )

		const std::string& display, // display from the config.ocio file  ( 1 "display" is a list of "view" )
		const std::string& view,    // view from the selected "display"

		float exposure, 
		float gamma,
		const std::string& file_path,

		bool useOCIO
		);



};








