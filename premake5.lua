-- cfg. = configuration
-- .buildcfg = configuration(Debug, Release, Dist,...)
-- .system = system(Windows,...)
-- .architecture = architecture(x86, x86_64,...)

-- prj. = project
-- .name



workspace "ShutdownPlanner"  -- solution dir
    architecture "x86_64"  --  architecture(x86, x86_64,...)

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ShutdownPlanner" --  project name
    location "ShutdownPlanner" --  project dir
    kind "WindowedApp" --  project kind(ConsoleApp, WindowedApp, SharedLib, StaticLib,...)
    language "C++"

    targetdir ("bins/bin/" .. outputdir .. "/%{prj.name}")   --  bin directory
    objdir ("bins/bin-int/" .. outputdir .. "/%{prj.name}")  --  bin-int directory

    files  -- files to include
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs  -- (C++/General/"Additional Include Directories")
    {
        "%{prj.name}/src",
        "%{prj.name}/Dependencies/wxWidgets/include",
        "%{prj.name}/Dependencies/wxWidgets/include/msvc",
    }

    libdirs  -- .lib directories (Linker/General/"Additional Library Directories")
    {
        "%{prj.name}/Dependencies/wxWidgets/lib/vc_x64_lib",

    }

    links  -- .lib's in .lib directory (Linker/Input/"Additional Dependencies")
    {
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "Off" --  Runtime Library (C++/Code Generation/Runtime Library) (On=MultiThreaded, Off=MultiThreadedDLL)
        systemversion "latest"
        buildoptions "/utf-8"

    filter "configurations:Debug"
        defines "IS_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "IS_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "IS_DIST"
        optimize "On"