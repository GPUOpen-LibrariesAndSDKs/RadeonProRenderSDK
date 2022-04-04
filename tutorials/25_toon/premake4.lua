project "25_toon"
    kind "ConsoleApp"
    location "../build"
    files { "../25_toon/**.h", "../25_toon/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../25_toon/**.h", "../25_toon/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++14"

	configuration {"x64"}
	links {"RadeonProRender64"}

    if os.istarget("linux") then
	    links {"pthread"}
    end
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
