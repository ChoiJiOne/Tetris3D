# How to use `HotReload.bat`

이 문서는 `HotReload.bat` 스크립트 사용방법에 대한 문서입니다.
<br><br>


## `HotReload.bat` 이란?

`HotReload.bat`는 Visual Studio 솔루션을 업데이트하는 배치 스크립트로 [Premake5](https://premake.github.io/)를 기반으로 구현되었습니다.  
<br><br>


## `HotReload.bat` 요구 사항

`HotReload.bat`를 사용하기 위해서는 다음이 필요합니다.
- Visual Studio 2019 or 2022
<br><br>


## `HotReload.bat` 사용 방법

`HotReload.bat`를 사용하기 위해서는 `Tetris3D` 폴더에서 `CMD`를 실행하고 다음 명령어를 입력합니다.
```
HotReload.bat <visual-studio-version>
```

만약, `Visual Studio 2019`를 사용하고 있다면 다음과 같이 입력합니다.
```
HotReload.bat vs2019
```

만약, `Visual Studio 2022`를 사용하고 있다면 다음과 같이 입력합니다.
```
HotReload.bat vs2022
```