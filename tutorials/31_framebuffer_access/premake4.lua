project "31_framebuffer_access"
    kind "ConsoleApp"
    location "../build"
    files { "../31_framebuffer_access/**.h", "../31_framebuffer_access/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../31_framebuffer_access/**.h", "../31_framebuffer_access/**.cpp","../common/common.cpp","../common/common.h"} }


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
    
