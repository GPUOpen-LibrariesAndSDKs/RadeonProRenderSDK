project "65_light_baking"
    kind "ConsoleApp"
    location "../build"
    files { "../65_light_baking/**.h", "../65_light_baking/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../65_light_baking/**.h", "../65_light_baking/**.cpp","../common/common.cpp","../common/common.h"} }


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
    
