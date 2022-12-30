workspace "Renderer"
        configurations { "Debug", "Release" }

function setupProject(name, mainFile, folders)
    project(name)
    kind "ConsoleApp"
    language "C++"
        targetdir "bin/${cfg.buildcfg}"

        -- add project include dirs here
        includedirs { ".", "vendor" }

        cppdialect "C++17"

        -- warnings "all"

        -- files here
        files { mainFile, table.unpack(folders) }

        -- link libraries
        links { "pthread" }

        filter "configurations:Debug"
                defines { "DEBUG" }
                symbols "On"

        filter "configurations:Release"
                optimize "On"
end

local rendererFiles = {
    "acceleration/**.hpp",  "acceleration/**.cpp",
    "color/**.hpp",         "color/**.cpp",
    "core/**.hpp",          "core/**.cpp",
    "geometry/**.hpp",      "geometry/**.cpp",
    "image/**.hpp",         "image/**.cpp",
    "math/**.hpp",          "math/**.cpp",
    "scene/**.hpp",         "scene/**.cpp",
    "material/**.hpp",      "material/**.cpp",
    "vendor/tinyexr/tinyexr.h",
    "vendor/tinyexr/deps/miniz/miniz.c",
    "vendor/tinyexr/deps/miniz/miniz.c",

    "pathtracer/**.hpp",    "pathtracer/**.cpp",
}

local photonMapperFiles = {
    "acceleration/**.hpp",  "acceleration/**.cpp",
    "color/**.hpp",         "color/**.cpp",
    "core/**.hpp",          "core/**.cpp",
    "geometry/**.hpp",      "geometry/**.cpp",
    "image/**.hpp",         "image/**.cpp",
    "math/**.hpp",          "math/**.cpp",
    "scene/**.hpp",         "scene/**.cpp",
    "material/**.hpp",         "material/**.cpp",
    "vendor/tinyexr/tinyexr.h",
    "vendor/tinyexr/deps/miniz/miniz.c",
    "vendor/tinyexr/deps/miniz/miniz.c",

    "pathtracer/**.hpp",    "pathtracer/**.cpp",
    "photonmapper/**.hpp",    "photonmapper/**.cpp",
}

local toneMapperFiles = {
    "color/**.hpp",         "color/**.cpp",
    "core/**.hpp",          "core/**.cpp",
    "image/**.hpp",         "image/**.cpp",
    "math/**.hpp",          "math/**.cpp",
}

local testFiles = { "*/**.hpp", "*/**.cpp" }

setupProject("Renderer", "renderer.cpp", rendererFiles )
setupProject("ToneMapper", "tonemapper.cpp", toneMapperFiles)
setupProject("PhotonMapper", "photonmapper.cpp", photonMapperFiles)
setupProject("Tests", "tests.cpp", testFiles)