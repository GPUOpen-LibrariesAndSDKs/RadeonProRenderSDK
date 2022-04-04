project "63_hybrid"
    kind "ConsoleApp"
    location "../build"
    files { "../63_hybrid/**.h", "../63_hybrid/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../63_hybrid/**.h", "../63_hybrid/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++14"

	configuration {"x64"}
	links {"RadeonProRender64", "RprLoadStore64"}
	

    if os.istarget("linux") then
		links {"pthread"}
    end

    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
