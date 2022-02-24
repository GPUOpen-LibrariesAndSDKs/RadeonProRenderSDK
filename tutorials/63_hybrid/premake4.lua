project "63_hybrid"
    kind "ConsoleApp"
    location "../build"
    files { "../63_hybrid/**.h", "../63_hybrid/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../63_hybrid/**.h", "../63_hybrid/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	links {"RadeonProRender64", "RprLoadStore64"}
	
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
