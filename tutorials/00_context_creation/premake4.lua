project "00_context_creation"
    kind "ConsoleApp"
    location "../build"
    files { "../00_context_creation/**.h", "../00_context_creation/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../00_context_creation/**.h", "../00_context_creation/**.cpp","../common/common.cpp","../common/common.h"} }

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
    
