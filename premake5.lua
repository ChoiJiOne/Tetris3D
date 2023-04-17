-- Tetris3D 게임 개발 작업 영역입니다.
workspace "Tetris3D"
    -- 빌드 구성 요소입니다.
    configurations { 
        "Debug", 
        "Release", 
        "Shipping" 
    }

    -- 플랫폼을 설정합니다.
    platforms { "Win64" }

    -- 솔루션 경로를 설정합니다.
    location "%{wks.name}"

    -- 게임 소스 경로를 설정합니다.
    game="%{wks.location}/../Game"

    -- 스크립트 파일 경로를 설정합니다.
    script="%{wks.location}/../Script"

    -- 셰이더 파일 경로를 설정합니다.
    shader="%{wks.location}/../Shader"

    -- 서드파티 라이브러리 경로를 설정합니다.
    thirdparty="%{wks.location}/../ThirdParty"

    -- 시작 프로젝트를 설정합니다.
    startproject "Game"

    -- 게임 프로젝트입니다.
    project "Game"
        -- 게임 프로젝트의 종류를 설정합니다.
        kind "ConsoleApp"

        -- 프로그래밍 언어를 설정합니다.
        language "C++"

        -- C++의 표준을 설정합니다.
        cppdialect "C++17"

        -- include 경로를 추가합니다.
        includedirs {
            "%{game}",
            "%{game}/Private",
            "%{game}/Public",
            "%{script}",
            "%{shader}",
            "%{thirdparty}",
        }

        -- file 경로를 추가합니다.
        files {
            "%{game}/*",
            "%{game}/Private/*",
            "%{game}/Public/*",
            "%{script}/*",
            "%{shader}/*",
            
            "%{thirdparty}/miniaudio/*",
            "%{thirdparty}/SDL2/*",
            "%{thirdparty}/stb/*",
        }

        -- 빌드 시 셰이더 컴파일은 비활성화합니다.
        filter { "files:**.hlsl" }
            flags { "ExcludeFromBuild" }
        filter { }

        -- 명령행 인수를 설정합니다.
        debugargs {
            "Crash=%{wks.location}..\\Crash\\",
            "Content=%{wks.location}..\\Content\\",
            "Shader=%{wks.location}..\\Shader\\",
        }

        -- 라이브러리를 연결합니다.
        links {
            "Dbghelp.lib",
            "d3d11.lib",
            "dxgi.lib",
            "d3dcompiler.lib",
        }

        -- 엔진 빌드의 구성 요소별 설정을 수행합니다.
        filter "configurations:Debug"
            defines { "DEBUG" }
            runtime  "Debug"
            optimize "Off"
            symbols "On"
            debugdir "%{thirdparty}/Debug"

            links {
                "%{thirdparty}/Debug/SDL2.lib",
                "%{thirdparty}/Debug/SDL2main.lib",
            }

        filter "configurations:Release"
            defines { "NDEBUG", "RELEASE" }
            runtime "Release"
            optimize "On"
            symbols "On"
            debugdir "%{thirdparty}/Release"

            links {
                "%{thirdparty}/Release/SDL2.lib",
                "%{thirdparty}/Release/SDL2main.lib",
            }

        filter "configurations:Shipping"
            defines { "NDEBUG", "SHIPPING" }
            runtime "Release"
            optimize "Full"
            symbols "Off"
            debugdir "%{thirdparty}/Release"

            links {
                "%{thirdparty}/Release/SDL2.lib",
                "%{thirdparty}/Release/SDL2main.lib",
            }