project "61_mesh_import"
    kind "ConsoleApp"
    location "../build"
    files { "../61_mesh_import/**.h", "../61_mesh_import/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../61_mesh_import/**.h", "../61_mesh_import/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++14"

    configuration {"x64"}
    links {"RadeonProRender64", "RprLoadStore64", "ProRenderGLTF"}

    if os.istarget("linux") then
        linkoptions "-fopenmp"
		links {"pthread"}
    end

    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
