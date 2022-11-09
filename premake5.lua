workspace "Renderer"
        configurations { "Debug", "Release" }

function setupProject(name, mainFile)
    project(name)
    kind "ConsoleApp"
    language "C++"
        targetdir "bin/${cfg.buildcfg}"

        -- add project include dirs here
        includedirs { "." }

        cppdialect "C++17"

        -- files here
        files { "*/**.cpp", "*/**.hpp", mainFile }

        -- link libraries
        links { "pthread" }

        filter "configurations:Debug"
                defines { "DEBUG" }
                symbols "On"

        filter "configurations:Release"
                optimize "On"
end

setupProject("Renderer", "renderer.cpp")
setupProject("ToneMapper", "tonemapper.cpp")
setupProject("Tests", "tests.cpp")