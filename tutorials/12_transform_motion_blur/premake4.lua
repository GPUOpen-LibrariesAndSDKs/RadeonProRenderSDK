project "12_transform_motion_blur"
    kind "ConsoleApp"
    location "../build"
    files { "../12_transform_motion_blur/**.h", "../12_transform_motion_blur/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../12_transform_motion_blur/**.h", "../12_transform_motion_blur/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"


	if os.istarget("linux") then
	    linkoptions {"-pthread"}
    end

	links {"RadeonProRender64"}
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
