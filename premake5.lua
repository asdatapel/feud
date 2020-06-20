workspace "Feud"
    configurations { "Debug", "Release" }
    systemversion "latest"
    language "C++"
    cppdialect "C++17"
    architecture "x86_64"

project "Common"
    location "Common"
    kind "StaticLib"
    language "C++"
    architecture "x64"

    targetdir "build/%{prj.name}/bin/%{cfg.buildcfg}"
    objdir "build/%{prj.name}/obj/%{cfg.buildcfg}"
    
    files { "%{prj.name}/include/**.hpp", "%{prj.name}/src/**.cpp" }
    includedirs {"%{prj.name}/include"}

    filter "configurations:Debug"
         defines { "DEBUG" }
         symbols "On" 
         
     filter "configurations:Release"
         defines { "NDEBUG" }
         optimize "On"

project "Client"
    location "Client"
    kind "ConsoleApp"
    language "C++"
    architecture "x64"

    targetdir "build/%{prj.name}/bin/%{cfg.buildcfg}"
    objdir "build/%{prj.name}/obj/%{cfg.buildcfg}"

    files { "%{prj.name}/include/**.hpp", "%{prj.name}/src/**.cpp" }
    includedirs {
        "%{prj.name}/include",
        "Common/include",

        "ext/nlohmann_json/single_include",
        "ext/glm",
        "ext/SFML/include",
        "ext/assimp/include",
        "ext/glew/include",
        "ext/freetype/include"
    }

    libdirs {
        "ext/glew/lib",
        "ext/SFML/lib",
    }

    links {
        "Common",

        "glew32s",
        "OpenGL32",
    }

   filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

        debugdir "./"

        libdirs {
            "ext/assimp/lib/Debug",
            "ext/freetype/Debug",
        }
    
        links {
            "assimp-vc141-mtd",
            "freetyped",
            "sfml-system-d",
            "sfml-window-d",
            "sfml-graphics-d",
        }

        postbuildcommands {
            "{COPY} ../ext/SFML/bin/*.dll %{cfg.targetdir}",
            "{COPY} ../ext/assimp/bin/Debug/*.dll %{cfg.targetdir}",
            "{COPY} ../ext/freetype/Debug/*.dll %{cfg.targetdir}",
        }

        
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Client"
    kind "ConsoleApp"
    language "C++"
    architecture "x64"
    
    targetdir "build/%{prj.name}/bin/%{cfg.buildcfg}"
    objdir "build/obj/%{cfg.buildcfg}"

    files { "%{prj.name}/include/**.hpp", "%{prj.name}/src/**.cpp" }
    includedirs {
        "%{prj.name}/include",
        "Common/include",

        "ext/SFML/include",
    }

    libdirs {
        "ext/SFML/lib",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

        debugdir "./"

        links {
            "sfml-network-d",
        }

        postbuildcommands {
            "{COPY} ../ext/SFML/bin/*.dll %{cfg.targetdir}",
        }
 
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"