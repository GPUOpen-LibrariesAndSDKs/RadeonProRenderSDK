project "32_gl_interop"
    kind "ConsoleApp"
    location "../build"
    files { "../32_gl_interop/**.h", "../32_gl_interop/**.cpp"} 
    includedirs{ "../../RadeonProRender/inc" } 

	if os.istarget("windows") then
    	includedirs{ "../../3rdParty/glew/include", "../../3rdParty/freeglut/include"} 	
	end
	
    buildoptions "-std=c++11"

	configuration {"x64"}

	links {"RadeonProRender64", "GLEW"}

	if os.istarget("windows") then
		links {"freeglut"}
	else
		links {"glut"}	
		links {"GL"}
	end

	if os.istarget("windows") then
		libdirs {"../../3rdParty/glew/lib/x64", "../../3rdParty/freeglut/lib/x64"}
	end
	
    configuration {"x64", "Debug"}
        targetdir "../Bin"
    configuration {"x64", "Release"}
        targetdir "../Bin"
    configuration {}