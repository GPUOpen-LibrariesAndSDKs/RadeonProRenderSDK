project "12_light_environment"
    kind "ConsoleApp"
    location "../build"
    files { "../12_light_environment/**.h", "../12_light_environment/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
