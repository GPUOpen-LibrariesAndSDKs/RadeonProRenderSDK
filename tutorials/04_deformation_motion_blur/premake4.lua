project "04_deformation_motion_blur"
    kind "ConsoleApp"
    location "../build"
    files { "../04_deformation_motion_blur/**.h", "../04_deformation_motion_blur/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 
    
    buildoptions "-std=c++11"

	configuration {"x64"}
	links {"RadeonProRender64"}
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}
    
