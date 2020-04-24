function fileExists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

solution "RprTracePlayer"
    configurations { "Debug", "Release" }           
    language "C++"
    flags { "NoMinimalRebuild", "EnableSSE", "EnableSSE2" }
    -- define common includes
    includedirs { ".","./build/", "../RadeonProRender/include" }

    -- perform OS specific initializations
    local targetName;
    if os.is("macosx") then
        targetName = "osx"
        platforms {"x64"}
    else
        platforms {"x64"}
    end
    
    if os.is("windows") then
        targetName = "win"
        defines{ "WIN32" }
        if _ACTION == "vs2010" then
            buildoptions { "/MP"  } --multiprocessor build
            defines {"_CRT_SECURE_NO_WARNINGS"}
            configuration {"Release"}
        end
    end

    if os.is("windows") then
        os.execute("mkdir \".\\build\\Rpr\\\"")
        os.execute("copy /Y \"..\\RadeonProRender\\inc\\*\" \".\\build\\Rpr\\\"")
        os.execute("mkdir \".\\build\\Rpr\\Math\\\"")
        os.execute("copy /Y \"..\\RadeonProRender\\inc\\Math\\*\" \".\\build\\Rpr\\Math\\\"")

        os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\RadeonProRender64.dll\" \".\\build\\\"")
        os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\RprLoadStore64.dll\" \".\\build\\\"")
        os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\Tahoe64.dll\" \".\\build\\\"")
        os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\OpenImageIO_RPR.dll\" \".\\build\\\"")
        os.execute("xcopy /Y \"..\\RadeonProRender\\binWin64\\RprSupport64.dll\" \".\\build\\\"")

        os.execute("xcopy /Y \".\\Trace\\rprTrace_data.bin\" \".\\build\\\"")
    end

    configuration "Debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
    configuration "Release"
        defines { "NDEBUG" }

    configuration {"x64", "Debug"}
        targetsuffix "64D"

    configuration {"x64", "Release"}
        targetsuffix "64"
    
    configuration {}
    if os.is("windows") then
        libdirs {"../RadeonProRender/libWin64" }
    end
    if os.is("macosx") then
        libdirs {"../RadeonProRender/libMacOs" }
    end
    links {"RadeonProRender64"}

    project "RprTracePlayer"
        kind "ConsoleApp"
        location "./build"
		targetdir "./build"
--        debugdir "Trace"
        files { "./Trace/*.h", "./Trace/*.cpp" } 
--        files { "../../Tahoe/Io/ImageIo/CompressedLoader/*.cpp" }
--        files { "../../Tahoe/Io/ImageIo/HdrLoader/*.cpp" }
        defines { "NON_WINDOW_TEST" }
        
        if os.is("windows") then
            links {"opengl32" }
        end
        if os.is("linux") then
            links {"GL", "X11", "Xi", "Xxf86vm", "Xrandr", "pthread" }
        end
        if os.is("macosx") then
            linkoptions{ "-framework Cocoa", "-framework OpenGL", "-framework IOKit" }
            -- for gtest
            buildoptions{"-DGTEST_USE_OWN_TR1_TUPLE=1"}
        end
        configuration {} -- set context to all configurations

