project "64_mesh_obj_demo"
    kind "ConsoleApp"
    location "../build"
    files { "../64_mesh_obj_demo/**.h", "../64_mesh_obj_demo/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../64_mesh_obj_demo/**.h", "../64_mesh_obj_demo/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	links {"RadeonProRender64", "RprLoadStore64"}


	if os.istarget("linux") then
	    linkoptions {"-pthread"}
    end

	
    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
