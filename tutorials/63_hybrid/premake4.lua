project "63_hybrid"
    kind "ConsoleApp"
    location "../build"
    files { "../63_hybrid/**.h", "../63_hybrid/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64", "RprLoadStore64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
