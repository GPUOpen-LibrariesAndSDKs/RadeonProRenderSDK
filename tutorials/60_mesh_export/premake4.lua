project "60_mesh_export"
    kind "ConsoleApp"
    location "../build"
    files { "../60_mesh_export/**.h", "../60_mesh_export/**.cpp"} 
    files { "../common/common.cpp","../common/common.h"}

    -- remove filters for Visual Studio
    vpaths { [""] = { "../60_mesh_export/**.h", "../60_mesh_export/**.cpp","../common/common.cpp","../common/common.h"} }


    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

    links {"RadeonProRender64", "RprLoadStore64", "ProRenderGLTF"}

    if os.istarget("linux") then
        linkoptions "-fopenmp"
    end

    filter "configurations:Debug"
        targetdir "../Bin"
    filter "configurations:Release"
        targetdir "../Bin"
    filter {}
    
