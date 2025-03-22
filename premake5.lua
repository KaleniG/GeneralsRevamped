WORKSPACEDIR = _WORKING_DIR
OUTPUTDIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SAGE"] = "../SAGE/Include"

workspace "GeneralsRevamped"
  startproject "GeneralsZeroHour"

  configurations 
  { 
    "Debug",
    "Release",
    "Final"
  }

  platforms 
  { 
    "Win64",
    "Win32"
  }

  filter { "platforms:Win64" }
    system "Windows"
    systemversion "latest"
    architecture "x86_64"

  filter { "platforms:Win32" }
    system "Windows"
    systemversion "latest"
    architecture "x86"

group "Dependencies"
group ""

group "Core"
  include "SAGE"
group ""

group "Implementations"
  include "Generals"
  include "GeneralsZeroHour"
group ""