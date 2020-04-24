project "31_framebuffer_access"
    kind "ConsoleApp"
    location "../build"
    files { "../31_framebuffer_access/**.h", "../31_framebuffer_access/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
