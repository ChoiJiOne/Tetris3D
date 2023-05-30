# Tetris3D *v0.0*
- 3D 테트리스 구현 프로젝트의 *v0.0*입니다.
<br><br>


## Platform

`Tetris3D` *v0.0*는 `Windows Platform`만 지원합니다.
<br><br>


## Requirements

`Tetris3D` *v0.0*를 유지보수 하기 위한 요구 사항은 다음과 같습니다.

- [git](https://git-scm.com/)
- [Visual Studio 2019 이상](https://visualstudio.microsoft.com/)
<br><br>


## Language

`Tetris3D` *v0.0*은 `C++17` 으로 구현되었습니다.  
> `Windows`에 특화된 코드도 존재하므로, `Linux`나 `MacOSX`에서는 빌드되지 않습니다.

<br><br>


## Source

`Tetris3D` *v0.0*의 소스코드를 얻기 위해서는 `CMD`에 다음 명령어를 입력하면 됩니다.
```
git clone https://github.com/ChoiJiOne/Tetris3D --single-branch -b v0.0
```
<br><br>


## Download & Play

실행 가능한 게임 실행 파일을 얻기 위해서는 [v0.0](https://github.com/ChoiJiOne/Tetris3D/releases/tag/v0.0)으로 이동한 뒤 `Tetris3D.zip`을 클릭합니다.
![](./Docs/how%20to%20play%20game/2023-05-17-07-33-00.png)

다운로드가 완료 되었으면 `Tetris3D.zip`의 압축을 해제합니다. 압축을 해제하면 다음과 같은 구조를 볼 수 있는데, `RunGame.bat`를 실행하면 게임을 실행할 수 있습니다.
![](./Docs/how%20to%20play%20game/2023-05-17-07-37-24.png)
<br><br>


## `GenerateProjectFiles.bat` 사용 방법

`GenerateProjectFiles.bat`를 사용하기 위해서는 `Tetris3D` 폴더에서 `CMD`를 실행하고 다음 명령어를 입력합니다.
```
GenerateProjectFiles.bat <visual-studio-version>
```

만약, `Visual Studio 2019`를 사용하고 있다면 다음과 같이 입력합니다.
```
GenerateProjectFiles.bat vs2019
```

만약, `Visual Studio 2022`를 사용하고 있다면 다음과 같이 입력합니다.
```
GenerateProjectFiles.bat vs2022
```
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
<br><br>


## `Build.bat` 사용 방법

`Build.bat`를 사용하기 위해서는 `Tetris3D` 폴더에서 `CMD`를 실행하고 다음 명령어를 입력합니다.
```
Build.bat <visual-studio-version> <mode>
```

이 스크립트가 지원하는 mode는 `Debug`, `Release`, `Shipping`입니다. 각 모드 별 특징은 다음과 같습니다.

| mode | description |
|:---|:---|
| Debug |  빌드 과정에서 최적화를 하지 않고, 디버그 정보 파일(.pdb)을 생성합니다. |
| Release |  빌드 과정에서 최적화는 하지만 디버그 정보 파일(.pdb)을 생성합니다. |
| Shipping | 빌드 과정에서 컴파일러가 할 수 있는 모든 최적화를 수행합니다. | 

만약, `Visual Studio 2019`를 사용하고 있다면 다음과 같이 입력합니다.
```
Debug 모드일 경우...
> Build.bat vs2019 Debug

Release 모드일 경우...
> Build.bat vs2019 Release

Shipping 모드일 경우...
> Build.bat vs2019 Shipping
```

만약, `Visual Studio 2022`를 사용하고 있다면 다음과 같이 입력합니다.
```
Debug 모드일 경우...
> Build.bat vs2022 Debug

Release 모드일 경우...
> Build.bat vs2022 Release

Shipping 모드일 경우...
> Build.bat vs2022 Shipping
```