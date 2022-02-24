project "05_basic_scene"
    kind "ConsoleApp"
    location "../build"
    files { "../05_basic_scene/**.h", "../05_basic_scene/**.cpp"}
    files { "../common/common.cpp","../common/common.h"} 

    -- remove filters for Visual Studio
    vpaths { [""] = { "../05_basic_scene/**.h", "../05_basic_scene/**.cpp","../common/common.cpp","../common/common.h"} }

    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	links {"RadeonProRender64"}
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
