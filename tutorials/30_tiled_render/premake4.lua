project "30_tiled_render"
    kind "ConsoleApp"
    location "../build"
    files { "../30_tiled_render/**.h", "../30_tiled_render/**.cpp", "../../3rdParty/stbi/**.h"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../30_tiled_render/**.h", "../30_tiled_render/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 

    buildoptions "-std=c++11"

    configuration {"x64"}
    links {"RadeonProRender64"}
    links {"RprLoadStore64"}

    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}

