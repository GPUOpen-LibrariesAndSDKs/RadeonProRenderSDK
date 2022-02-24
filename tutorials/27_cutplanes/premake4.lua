project "27_cutplanes"
    kind "ConsoleApp"
    location "../build"
    files { "../27_cutplanes/**.h", "../27_cutplanes/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../27_cutplanes/**.h", "../27_cutplanes/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	links {"RadeonProRender64"}
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
