project "61_mesh_import"
    kind "ConsoleApp"
    location "../build"
    files { "../61_mesh_import/**.h", "../61_mesh_import/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../61_mesh_import/**.h", "../61_mesh_import/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

    links {"RadeonProRender64", "RprLoadStore64", "ProRenderGLTF"}

    if os.istarget("linux") then
        linkoptions "-fopenmp"
    end

	if os.istarget("linux") then
	    linkoptions {"-pthread"}
    end


    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
