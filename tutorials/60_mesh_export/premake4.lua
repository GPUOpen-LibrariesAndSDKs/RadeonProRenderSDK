project "60_mesh_export"
    kind "ConsoleApp"
    location "../build"
    files { "../60_mesh_export/**.h", "../60_mesh_export/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64", "RprLoadStore64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
