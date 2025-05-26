workspace "Cogni-draw"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Cogni-draw"

project "Cogni-draw"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
    objdir    ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "Cogni-draw/src/**.cpp",
        "Cogni-draw/vendor/glad/glad.c",
        "Cogni-draw/vendor/imgui/**.cpp"
    }

    includedirs {
        "Cogni-draw/include",
        "Cogni-draw/Libraries/include",
        "Cogni-draw/vendor/",
        "Cogni-draw/Libraries/include/imgui",
    }

    libdirs { "Cogni-draw/libraries/lib" }
    links {
        "opengl32",
        "assimp-vc143-mtd",
        "glfw3"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"