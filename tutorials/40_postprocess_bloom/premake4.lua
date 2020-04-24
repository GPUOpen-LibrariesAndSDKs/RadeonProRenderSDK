project "40_postprocess_bloom"
    kind "ConsoleApp"
    location "../build"
    files { "../40_postprocess_bloom/**.h", "../40_postprocess_bloom/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
