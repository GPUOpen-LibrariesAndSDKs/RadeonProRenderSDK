project "25_toon"
    kind "ConsoleApp"
    location "../build"
    files { "../25_toon/**.h", "../25_toon/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../25_toon/**.h", "../25_toon/**.cpp","../common/common.cpp","../common/common.h"} }


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
    
