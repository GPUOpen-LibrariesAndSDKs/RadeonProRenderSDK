project "62_matball_demo"
    kind "ConsoleApp"
    location "../build"
    files { "../62_matball_demo/**.h", "../62_matball_demo/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64", "RprLoadStore64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
