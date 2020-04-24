project "23_material_uber_sss"
    kind "ConsoleApp"
    location "../build"
    files { "../23_material_uber_sss/**.h", "../23_material_uber_sss/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64" }
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
