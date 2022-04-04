project "03_parameters_enumeration"
    kind "ConsoleApp"
    location "../build"
    files { "../03_parameters_enumeration/**.h", "../03_parameters_enumeration/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}
    files { "../../RadeonProRender/rprTools/RPRStringIDMapper.cpp","../../RadeonProRender/rprTools/RPRStringIDMapper.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { 
		"../03_parameters_enumeration/**.h", "../03_parameters_enumeration/**.cpp",
		"../common/common.cpp","../common/common.h",
		"../../RadeonProRender/rprTools/RPRStringIDMapper.cpp", "../../RadeonProRender/rprTools/RPRStringIDMapper.h"
	} }

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
    
