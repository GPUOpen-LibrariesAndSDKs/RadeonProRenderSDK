function fileExists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

solution "Tutorials"
	configurations { "Debug", "Release" }    		
	language "C++"
	flags { "NoMinimalRebuild" }
    vectorextensions "SSE"
    vectorextensions "SSE2"
    -- define common includes
    includedirs { ".","../RadeonProRender/include" }

    -- perform OS specific initializations
    local targetName;
    if os.istarget("macosx") then
        targetName = "osx"
		platforms {"x64"}
	else
		platforms {"x64"}
	end
	
    if os.istarget("windows") then
        targetName = "win"
		defines{ "WIN32" }
        if _ACTION == "vs2010" then
            buildoptions { "/MP"  } --multiprocessor build
            defines {"_CRT_SECURE_NO_WARNINGS"}
            configuration {"Release"}
        end
	end

	if os.istarget("windows") then
		os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\RadeonProRender64.dll\" \".\\Bin\\\"")
		os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\RprLoadStore64.dll\" \".\\Bin\\\"")
		os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\Tahoe64.dll\" \".\\Bin\\\"")
		os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\RprSupport64.dll\" \".\\Bin\\\"")
		os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\Hybrid.dll\" \".\\Bin\\\"")
	end

    configuration "Debug"
		defines { "_DEBUG" }
		symbols "On"
	configuration "Release"
		defines { "NDEBUG" }

	configuration {"x64", "Debug"}
		targetsuffix "64D"

	configuration {"x64", "Release"}
		targetsuffix "64"
    
    configuration {} -- back to all configurations
	if os.istarget("windows") then
		libdirs {"../RadeonProRender/libWin64" }
	end
	if os.istarget("linux") then
		defines{ "__LINUX__" }
		libdirs {"../RadeonProRender/binUbuntu18" }
	end
	if os.istarget("macosx") then
		libdirs {"../RadeonProRender/libMacOs" }
	end

	include "00_context_creation"
	include "01_camera_setup"
	include "02_mesh"
	include "03_instance"
	include "10_light_point"
	include "11_light_mesh"
	include "12_light_environment"
	include "20_material_textured"
	include "21_material_layered"
	include "22_material_uber"
	include "23_material_uber_sss"
	include "30_tiled_render"
	include "31_framebuffer_access"
	include "32_gl_interop"
	include "33_aov"
	include "40_postprocess_bloom"
	include "41_postprocess_gamma"
	include "50_curve"
	include "51_volume"
	include "60_mesh_export"
	include "61_mesh_import"
	include "62_matball_demo"
	include "63_hybrid"

	if fileExists("./MultiTutorials/MultiTutorials.lua") then
		dofile("./MultiTutorials/MultiTutorials.lua")
	end
		
	
	if fileExists("./parameters_enumeration/parameters_enumeration.lua") then
		dofile("./parameters_enumeration/parameters_enumeration.lua")	
	end


