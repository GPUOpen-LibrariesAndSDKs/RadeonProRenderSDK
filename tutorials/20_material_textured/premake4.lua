project "20_material_textured"
    kind "ConsoleApp"
    location "../build"
    files { "../20_material_textured/**.h", "../20_material_textured/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
