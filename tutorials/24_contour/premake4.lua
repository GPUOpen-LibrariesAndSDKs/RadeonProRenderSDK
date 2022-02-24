project "24_contour"
    kind "ConsoleApp"
    location "../build"
    files { "../24_contour/**.h", "../24_contour/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../24_contour/**.h", "../24_contour/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	links {"RadeonProRender64"}


	if os.istarget("linux") then
	    linkoptions {"-pthread"}
    end


    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
