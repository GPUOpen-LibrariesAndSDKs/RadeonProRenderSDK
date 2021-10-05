project "00_context_creation"
    kind "ConsoleApp"
    location "../build"
    files { "../00_context_creation/**.h", "../00_context_creation/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../00_context_creation/**.h", "../00_context_creation/**.cpp","../common/common.cpp","../common/common.h"} }

    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
