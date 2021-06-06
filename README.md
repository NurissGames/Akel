# Akel

![Logo](https://github.com/Kbz-8/Akel/blob/dev/src/assets/logo.png)

Akel is a game engine made for fun by a young french man.
Currently at an early stage, it is still in development and still needs some work to turn it into something.
A graphical interface (like Unity or UnrealEngine) will soon be in development, facilitating the creation of games. However, it will be necessary to wait for some time until this one is sufficiently developed (as well as the engine itself) to make games "easily".

# Getting started
Developed mainly on Linux, Akel is, however, cross-platform and can be used on Windows, although it has not been tested on this OS (no information about MacOS, I have to find out). It is used with [Premake5](https://premake.github.io/). GCC 9 is required because it uses C++17 features. Dependencies are provided (SDL2, OpenGL, Vulkan, GML) with the code.

* Start by clonning the repo in your project's folder `git clone https://github.com.Kbz-8/Akel`.
* <details> <summary>Create a "premake5.lua" file in your project's directory, here's an example of what it might look like if your AtlasEnigne folder is in the same directory as your premake5.lua file: </summary>

        include "./Akel/vendor/premake/premake_customization/solution_items.lua"

        workspace "test"
          startproject "Akel"
          startproject "engineTester"

          configurations
          {
            "Debug",
            "Release"
          }

          solution_items ".editorconfig"

          flags "MultiProcessorCompile"
          outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

        include "Akel"

        project "engineTester"
          kind "WindowedApp"
          language "C++"
          cppdialect "C++17"
          staticruntime "on"
          icon "%{prj.location}/Akel/src/assets/logo.png"

          targetdir ("%{prj.location}")
          objdir ("%{prj.location}/bin-int/" .. outputdir)

          files {"**.cpp", "**.hpp", "**.h", "**.c""}

          libdirs "./Akel/build/Debug-linux-"
          links "Akel"

          includedirs 
          {
            "%{prj.location}/Akel/src",
            "%{prj.location}/Akel/libs/include"
          }
        
   </details>
* <details> <summary>Use the "premake5" executable provided in "vendor/premake/bin" (choose carefully according to your system (32bits, 64bits)), then launch it in the same folder as your premake5.lua file with the right option depending on how you compile your project :</summary>
    
    Premake5 options | Consequences
    ---------------- | ------------
    vs2019 | Generate Visual Studio 2019 project files
    vs2017 | Generate Visual Studio 2017 project files
    vs2015 | Generate Visual Studio 2015 project files
    vs2013 | Generate Visual Studio 2013 project files
    vs2012 | Generate Visual Studio 2012 project files
    vs2010 | Generate Visual Studio 2010 project files
    vs2008 | Generate Visual Studio 2008 project files
    vs2005 | Generate Visual Studio 2005 project files
    gmake | Generate GNU Makefiles (This generator is deprecated by gmake2)
    gmake2 | Generate GNU Makefiles (including Cygwin and MinGW)
    xcode4 | XCode projects
    codelite | CodeLite projects
    
</details>
* Build your project and enjoy or cry depending on if it compiles correctly or if there are 400 errors


# Akel useful features
* Text management language [ELTM](https://github.com/Kbz-8/Akel/tree/dev/src/Modules/ELTM)
* Simple but efficient error management system 
* System information retrieval
  * CPU infos (hardware and software)
  * GPU infos (hardware and software)
* Rendering system with Vulkan

### Contact
bilbo.sacquet@orange.fr
