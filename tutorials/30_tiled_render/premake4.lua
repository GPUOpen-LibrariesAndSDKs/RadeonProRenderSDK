project "30_tiled_render"
    kind "ConsoleApp"
    location "../build"
    files { "../30_tiled_render/**.h", "../30_tiled_render/**.cpp", "../../3rdParty/stbi/**.h"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../30_tiled_render/**.h", "../30_tiled_render/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 

    buildoptions "-std=c++11"


	if os.istarget("linux") then
	    linkoptions {"-pthread"}
    end

	links {"RadeonProRender64","RprLoadStore64"}
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}





