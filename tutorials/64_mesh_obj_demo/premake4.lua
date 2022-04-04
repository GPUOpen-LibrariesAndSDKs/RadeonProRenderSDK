project "64_mesh_obj_demo"
    kind "ConsoleApp"
    location "../build"
    files { "../64_mesh_obj_demo/**.h", "../64_mesh_obj_demo/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../64_mesh_obj_demo/**.h", "../64_mesh_obj_demo/**.cpp","../common/common.cpp","../common/common.h"} }


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
    
