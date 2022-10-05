workspace "Renderer"
        configurations { "Debug", "Release" }

function setupProject(name, mainFile)
    project(name)
    kind "ConsoleApp"
    language "C++"
        targetdir "bin/${cfg.buildcfg}"

        -- add project include dirs here
        includedirs { "." }

        -- files here
        files { "*/**.cpp", "*/**.hpp", mainFile }

        filter "configurations:Debug"
                defines { "DEBUG" }
                symbols "On"

        filter "configurations:Release"
                optimize "On"
end

setupProject("Renderer", "renderer.cpp")
setupProject("ToneMapper", "tonemapper.cpp")