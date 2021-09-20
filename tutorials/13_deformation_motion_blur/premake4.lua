project "13_deformation_motion_blur"
    kind "ConsoleApp"
    location "../build"
    files { "../13_deformation_motion_blur/**.h", "../13_deformation_motion_blur/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../13_deformation_motion_blur/**.h", "../13_deformation_motion_blur/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
