project "00_context_creation"
    kind "ConsoleApp"
    location "../build"
    files { "../00_context_creation/**.h", "../00_context_creation/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../00_context_creation/**.h", "../00_context_creation/**.cpp","../common/common.cpp","../common/common.h"} }

    includedirs{ "../../RadeonProRender/inc" } 
    

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
    
