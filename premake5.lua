workspace "tkplib"
	architecture "x86_64"
	configurations { "Debug", "Release"}

include 'src/external/glew/glew.lua'

project "tkplib"
	kind "SharedLib"
	language "C"
	cdialect "c99"
	targetdir "bin"
	includedirs { "src" }
	files { "src/*.c", "src/*.h" }

	warnings "Extra"

	filter "system:linux" 
		links { "X11", "Xrandr", "GL", "GLX", "glew" }

	filter "system:windows"
		links { "gdi32", "user32", "shell32", "opengl32", "glu32", "glew" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"


project "triangle"
	kind "ConsoleApp"
	language "C"
	cdialect "c99"
	targetdir "bin"
	files { "examples/triangle/**.c", "examples/triangle/**.h" }

	warnings "Extra"

	links { "tkplib" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

