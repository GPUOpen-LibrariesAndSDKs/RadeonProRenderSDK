project "51_volume"
    kind "ConsoleApp"
    location "../build"
    files { "../51_volume/**.h", "../51_volume/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../51_volume/**.h", "../51_volume/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	links {"RadeonProRender64"}
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
