# project directory

이 문서는 프로젝트 디렉토리 구조에 대한 문서입니다.
<br><br>


## 디렉토리 구조

다음은 `Tetris3D`의 *v0.0* 버전 프로젝트 디렉토리 구조입니다.
```
├─Bin
├─Content
├─Crash
├─Docs
├─Game
│  ├─Private
│  └─Public
├─Script
├─Shader
└─ThirdParty
    ├─Debug
    ├─json
    ├─miniaudio
    ├─Release
    ├─SDL2
    └─stb
```

### Bin

`Bin` 폴더는 프로젝트에서 사용하는 바이너리 파일이 있는 폴더입니다. 여기서는 Visual Studio 세팅에 사용하는 `Premake5`와 HLSL 셰이더를 컴파일하는 `셰이더 컴파일러`가 있습니다.

### Content

`Content` 폴더는 게임에서 사용할 리소스가 있는 폴더입니다. 주로 텍스처, 폰트, 사운드, 설정 파일 등이 있습니다.

### Crash

`Crash` 폴더는 개발 혹은 플레이 중 발생한 크래시 덤프 파일을 수집하는 폴더입니다. 이 폴더는 원격 저장소에 push 되지 않습니다.

### Docs

`Docs` 폴더는 프로젝트를 진행하면서 작성한 문서 폴더입니다. 주로 스크립트 사용법, 아이디어 설명, 프로젝트 제안서, 버그 리포트 등이 있습니다.

### Game

`Game` 폴더는 게임 핵심 로직이 구현된 C++ 소스 파일 폴더입니다. 폴더 하위에는 `Public`과 `Private`이 있는데, `Public`에는 헤더 파일이 있고 `Private`에는 구현 파일이 있습니다.

### Script

`Script` 폴더는 프로젝트에서 사용하는 스크립트가 있는 폴더입니다. 주로 빌드 스크립트, Visual Studio 솔루션 세팅 자동화 스크립트와 같은 자동화 기능 스크립트가 있습니다.

### Shader

`Shader` 폴더는 렌더링 시 사용할 HLSL이 있는 폴더입니다. 단순 텍스처 렌더링이나 라이팅, 그림자 효과 등을 수행하는 셰이더가 있습니다.

### ThirdParty

`ThirdParty` 폴더는 프로젝트에서 사용하는 서드 파티 라이브러리 폴더입니다. 하위에 `Debug`와 `Release`가 있는데, 여기에는 `DLL`이나 `Static Library` 같은 바이너리 파일들이 존재합니다.