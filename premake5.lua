solution "NebuleuseClient"
   configurations { "Debug", "Release" }

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
   
   filter "configurations:Debug"
      defines { "DEBUG" }
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
   libdirs { "lib/"}
   
   filter "configurations:Debug"
      links { "libcurl_a_debug" }
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      links { "libcurl_a" }
      defines { "NDEBUG" }
      optimize "On"