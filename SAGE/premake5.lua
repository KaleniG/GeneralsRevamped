project "SAGE"
  kind "StaticLib"
  language "C++"
  cppdialect "C++latest"
  staticruntime "On"

  targetdir (WORKSPACEDIR .. "/Bin/" .. OUTPUTDIR .. "/%{prj.name}")
  objdir (WORKSPACEDIR .. "/Bin-Int/" .. OUTPUTDIR .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "Source/pch.cpp"

  files
  {
    "%{IncludeDir.json}/**.hpp",
    "%{IncludeDir.glm}/**.hpp",
    "%{IncludeDir.glm}/**.inl",
    "Source/**.cpp",
    "Source/**.h",
    "premake5.lua"
  }

  includedirs
  {
    "Source",
    "%{IncludeDir.json}",
    "%{IncludeDir.glm}"
  }

  filter { "configurations:Debug" }
    symbols "On"
    optimize "Off"
    defines
    {
      "CONFIG_DEBUG"
    }

  filter { "configurations:Release" }
    symbols "Off"
    optimize "Full"
    defines
    {
      "CONFIG_RELEASE"
    }

  filter { "configurations:Final" }
    symbols "Off"
    optimize "Full"
    defines
    {
      "CONFIG_FINAL"
    }