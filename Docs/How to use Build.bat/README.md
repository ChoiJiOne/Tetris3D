# Hot to use `Build.bat`

이 문서는 `Build.bat` 스크립트 사용방법에 대한 문서입니다.
<br><br>


## `Build.bat` 이란?

`Build.bat`는 `msbuild.exe`를 기반으로 전체 솔루션을 빌드하는 배치 스크립트입니다.
<br><br>


## `Build.bat` 요구 사항

`Build.bat`를 사용하기 위해서는 다음이 필요합니다.
- Visual Studio 2019 or 2022
- `msbuild.exe` 환경 변수 등록

> ※ `msbuild.exe` 환경 변수 등록을 하지 않으면 `Build.bat` 스크립트는 동작하지 않습니다!
<br><br>


## `msbuild.exe` 환경 변수 등록

`msbuild.exe` 환경 변수 등록 절차는 다음과 같습니다. 만약 환경 변수가 이미 등록되어 있다면 이 과정은 생략해도 됩니다.

### 1단계