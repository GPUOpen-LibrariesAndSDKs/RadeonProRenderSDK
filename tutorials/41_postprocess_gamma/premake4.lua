project "41_postprocess_gamma"
    kind "ConsoleApp"
    location "../build"
    files { "../41_postprocess_gamma/**.h", "../41_postprocess_gamma/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
