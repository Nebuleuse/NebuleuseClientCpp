solution "NebuleuseClient"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64" }

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
	
	filter { "platforms:Win32" }
		targetname "Nebuleuse.x86"
		system "Windows"
		architecture "x32"

	filter { "platforms:Win64" }
		targetname "Nebuleuse.x64"
		system "Windows"
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
   
   	filter { "platforms:Win32" }
		system "Windows"
		architecture "x32"

	filter { "platforms:Win64" }
		system "Windows"
		architecture "x64"
   
   filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
	  
	filter { "configurations:Release", "platforms:Win32"}
		links { "libcurl_a.x86" }
	filter { "configurations:Release", "platforms:Win64"}
		links { "libcurl_a.x64" }
	filter { "configurations:Debug", "platforms:Win32"}
		links { "libcurl_a_debug.x86" }
	filter { "configurations:Debug", "platforms:Win64"}
		links { "libcurl_a_debug.x64" }