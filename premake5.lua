solution "NebuleuseClient"
   configurations { "Debug", "Release" }
   platforms { "x86", "x64" }

project "Nebuleuse"
   kind "StaticLib"
   language "C++"
   targetdir "lib/%{cfg.buildcfg}"
   
   includedirs { "include", 
				 "curl/builds/%{cfg.buildcfg}/include",
				 "rapidjson/include"}
   files { "src/**.cpp", "include/**.h" }
   
   libdirs { "curl/builds/%{cfg.buildcfg}/lib" }
   
   defines "CURL_STATICLIB"

   configuration "linux"
      buildoptions { "-std=c++11" }
      linkoptions { "-pthread" }
      includedirs{"/usr/local/include", "/usr/include/x86_64-linux-gnu/c++/4.9/", "/usr/include/x86_64-linux-gnu"}

	
	filter { "platforms:x86" }
		targetname "Nebuleuse.x86"
		architecture "x32"

	filter { "platforms:x64" }
		targetname "Nebuleuse.x64"
		architecture "x64"
   
   filter "configurations:Debug"
      defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=2" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "Tester"
   kind "ConsoleApp"
   language "C++"
   targetdir "Tester/bin/"
   
   includedirs { "include" }
   files { "Tester/*.cpp" }
   links { "Nebuleuse" }
   libdirs { "lib/%{cfg.buildcfg}"}
   
   defines "CURL_STATICLIB"
   
   filter { "platforms:x86" }
		architecture "x32"

	filter { "platforms:x64" }
		architecture "x64"
   
   filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
   
   filter {"system:not windows"}
      buildoptions { "-std=c++11" }
      linkoptions { "-pthread" }
      links { "curl" }
      includedirs{"/usr/local/include/curl"}

  	filter { "configurations:Release", "platforms:x86", "system:windows"}
  		links { "libcurl_a.x86" }
  	filter { "configurations:Release", "platforms:x64", "system:windows"}
  		links { "libcurl_a.x64" }
  	filter { "configurations:Debug", "platforms:x86", "system:windows"}
  		links { "libcurl_a_debug.x86" }
  	filter { "configurations:Debug", "platforms:x64", "system:windows"}
   		links { "libcurl_a_debug.x64" }