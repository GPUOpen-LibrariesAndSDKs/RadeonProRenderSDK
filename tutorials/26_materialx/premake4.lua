project "26_materialx"
    kind "ConsoleApp"
    location "../build"
    files { "../26_materialx/**.h", "../26_materialx/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../26_materialx/**.h", "../26_materialx/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	links {"RadeonProRender64"}
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
