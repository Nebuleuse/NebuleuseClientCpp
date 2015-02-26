solution "Nebuleuse"
   configurations { "Debug", "Release" }

project "Nebuleuse"
   kind "StaticLib"
   language "C++"
   targetdir "Nebuleuse/lib/%{cfg.buildcfg}"
   
   includedirs { "Nebuleuse/include", 
				 "Nebuleuse/curl/builds/%{cfg.buildcfg}/include",
				 "Nebuleuse/rapidjson/include"}
   files { "Nebuleuse/src/**.cpp", "Nebuleuse/include/**.h" }
   
   libdirs { "Nebuleuse/curl/builds/%{cfg.buildcfg}/lib" }
   
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
   targetdir "Testes/bin/"
   
   includedirs { "Nebuleuse/include" }
   files { "Tester/*.cpp" }
   links { "Nebuleuse" }
   
   filter "configurations:Debug"
      links { "libcurl_a_debug" }
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      links { "libcurl_a" }
      defines { "NDEBUG" }
      optimize "On"