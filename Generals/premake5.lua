project "Generals"
  language "C++"
  cppdialect "C++latest"
  staticruntime "On"

  targetdir (WORKSPACEDIR .. "/Bin/" .. OUTPUTDIR .. "/%{prj.name}")
  objdir (WORKSPACEDIR .. "/Bin-Int/" .. OUTPUTDIR .. "/%{prj.name}")

  files
  {
    "Source/**.cpp",
    "Source/**.h",
    "premake5.lua"
  }

  includedirs
  {
    "Source",
    "%{IncludeDir.SAGE}"
  }

  links
  {
    "SAGE"
  }

  filter { "configurations:Debug" }
    symbols "On"
    optimize "Off"
    kind "ConsoleApp"
    defines
    {
      "CONFIG_DEBUG"
    }

  filter { "configurations:Release" }
    symbols "Off"
    optimize "Full"
    kind "ConsoleApp"
    defines
    {
      "CONFIG_RELEASE"
    }

  filter { "configurations:Final" }
    symbols "Off"
    optimize "Full"
    kind "WindowedApp"
    entrypoint "mainCRTStartup"
    defines
    {
      "CONFIG_FINAL"
    }