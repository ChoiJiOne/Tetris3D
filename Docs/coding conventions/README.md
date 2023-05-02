# coding conventions

이 문서는 코딩 표준에 대한 문서입니다.
<br><br>


## 동기

코딩 표준을 정하는 데는 여러 가지 이유가 있지만, 이 프로젝트에서 코딩 표준을 정하는 이유는 코드의 가독성을 향상 시키기위함 입니다. 이를 위해 이전 프로젝트에서는 일부분을 제외한 언리얼 코딩 표준을 지켜왔고, 언리얼은 모든 이름의 첫 글자는 대문자를 사용하는 파스칼 형식을 사용하고 이름을 명명할 경우 다음과 같은 특정 접두사가 존재합니다.

1. 템플릿은 T
2. `UObject`를 상속하면 U
3. `AActor`를 상속하면 A
4. `SWidget`을 상속하면 S
5. 추상 인터페이스의 경우 I
6. 열거형(Enum)의 접두사는 E
7. 부울(Boolean) 변수의 접두사는 b
8. 그 외의 대부분의 클래스의 접두사는 F, 일부 서브 시스템에서는 다른 글자 사용

1번, 5번, 6번, 7번을 지키는 데는 문제가 없으나 그 외를 지키는 것은 생각보다 문제가 되어 지키지 않았습니다. 하지만 규칙을 지켰을 때 다음과 같은 문제가 발생합니다.

```
// 이전의 Tetris2D 프로젝트의 테트로미노입니다.
class Tetromino : public GameObject
{
    ...
}
...
std::unique_ptr<Tetromino> TetrominoObject = std::make_unique<Tetromino>(...);
```

소문자를 혀용한다면 `TetrominoObject` 대신 `tetromino`라고 작성하여 불필요하게 변수의 이름이 길어지는 것을 방지할 수 있지만 `모든 이름의 첫 글자는 대문자를 사용하는 파스칼 형식을 사용`한다는 언리얼 규칙으로 인해 이전의 프로젝트들은 많은 부분에서 변수의 이름이 긴 것들이 많아 오히려 가독성을 해치는 경우가 있었습니다. 이에 맹목적으로 언리얼 코딩 표준을 따르는 것이 아닌, 다양한 코딩 표준들을 참고하여 이 프로젝트에 적합한 코딩 표준을 정하게 되었습니다.
<br><br>


## 코딩 표준

이 프로젝트에서 정한 코딩 표준은 다음과 같습니다.
- **클래스 이름**, **구조체 이름**, 클래스 및 구조체 내의 **메서드 이름**은 모두 `파스칼 형식`을 사용합니다.
    ```
    class Monster { ... };
    struct Vector { ... };

    class Player 
    {
        ...
        void DoSomething(...);
        ...
    }
    ```
- **지역 변수**, **멤버 변수**, **함수의 매개 변수**와 같이 변수 이름은 모두 `캐멀 형식`을 사용합니다.
    ```
    class Player
    {
        ...
        void DoSomething(int value)
        {
            int some = value + 10;
            ...
        }
        ...

        int area_;
    }
    ```
- 클래스의 멤버 변수 이름 뒤에 언더바(_)를 붙입니다.
    ```
    class Rect
    {
        ...
        int width_;
        int height_;
        int area_;
    }
    ```
- 부울 변수(boolean) 이름은 앞에 b 접두사를 붙입니다.
    ```
    bool bIsDone;
    bool bCanMove;
    bool bEnableMode;
    ```
- 부울 값을 반환하는 모든 함수는 true/false 질문을 합니다.
    ```
    bool IsDoneGame();
    bool CanMove();
    bool ShouldClearBuffer();
    ```
- 열거형을 선언할 때는 C 스타일의 `enum` 대신 `enum class` 를 사용하고 앞에 E 접두사를 붙입니다.
    ```
    enum class EFlag
    {
        NONE = 0x00,
        ONE = 0x01,
    }
    ```
- `for` 문에서 인덱스를 선언할 때는 `i`, `j`, `k` 같이 한글자로 된 변수는 사용하지 않습니다.
    ```
    // 사용하지 않는 예
    for(int i = 0; i < size; ++i)
    {
        ...
    }

    // 사용하는 예
    for(int objectIndex = 0; objectIndex < objects.size(); ++objectIndex)
    {

    }
    ```
- `for` 문에서 인덱스를 선언하는 예전 방식보다 `for range` 같이 현대적인 방법을 사용합니다.
    ```
    // 권장하지 않는 방식
    for(int objectIndex = 0; objectIndex < objects.size(); ++objectIndex)
    {

    }

    // 권장하는 방식
    for(const auto& object : objects)
    {
        ...
    }
    ```
- 클래스 혹은 클래스의 메서드에 주석을 작성할 때는 반드시 JavaDoc 포멧으로 작성한다.
    ```
    /**
     * @brief 파일 처리에 대한 핼퍼 기능을 수행합니다.
     * 
     * @note 이 클래스는 멤버 함수 모두가 정적인 정적(static) 클래스입니다.
     */
    class FileHelper
    {
    public:
        /**
        * @brief 파일을 읽고 버퍼에 저장합니다.
        *
        * @note 파일 경로의 문자열은 UTF-8 입니다.
        *
        * @see https://learn.microsoft.com/ko-kr/windows/win32/api/fileapi/nf-fileapi-createfilea
        *
        * @param path 파일의 경로입니다.
        * @param outBuffer[out] 읽은 파일을 저장할 버퍼입니다.
        *
        * @throws
        * 파일 생성에 실패하면 C++ 표준 예외를 던집니다.
        * 파일 읽기에 실패하면 C++ 표준 예외를 던집니다.
        */
        static inline void ReadBufferFromFile(const std::string& path, std::vector<uint8_t>& outBuffer)
        {
            ...
        }
    ```
- C스타일의 `NULL` 대신 C++의 `nullptr`을 사용합니다.
    ```
    ...
    window_ = SDL_CreateWindow(title.c_str(), x, y, width, height,static_cast<uint32_t>(flags));
	CHECK((window_ != nullptr), "failed to create window...");
    ...
    ```
- C/C++의 동적 할당 대신 C++의 스마트 포인터를 사용합니다.
    ```
    // 권장하지 않는 방식
    int* arr = (int*)malloc(sizeof(int) * 10);
    int* arr = new int[10];

    // 권장하는 방식
    std::unique_ptr<Object> object = std::make_unique<Object>();
    ```
- C스타일의 캐스팅 대신 C++의 캐스팅을 사용합니다.
    ```
    // 권장하지 않는 방식
    int* arr = (int*)(otherArray);

    // 권장하는 방식
    int* arr = reinterpret_cast<int*>(otherArray);
    std::size_t bufferSize = static_cast<std::size_t>(size);
    ```
- 상수 또는 `#define` 으로 정의된 상수 이름은 모든 글자를 대문자로 하고 띄어야 할 경우 언더바(_)로 구분합니다.
    ```
    const int SOME_CONST = 0;
    #define SOMETHING 1
    ```
- 전역 함수를 사용하는 대신 클래스의 정적 메서드를 활용합니다.
    ```
    class FileHelper
    {
        static inline void ReadBufferFromFile(const std::string& path, std::vector<uint8_t>& outBuffer)
        {
            ...
        }
    }
    ```
- 매크로 함수의 경우, 모든 글자를 대문자로 하고 띄어야 할 경우 언더바(_)로 구분합니다.
    ```
    #ifndef DO_SOMETHING
    #define DO_SOMETHING(PARAM1, PARAM2, ...)
    ...
    #endif
    ```
- 클래스를 선언할 때는 `public`, `protected`, `private` 순서로 합니다.
    ```
    class Object
    {
    public:
        ...

    protected:
        ...

    private:
        ...
    }
    ```
- 모든 헤더 파일에는 `#pragma once`를 추가합니다.
- double이 아닌 float를 사용하면 반드시 뒤에 f를 붙여줍니다.
    ```
    float pi = 3.14f;
    ```
- 클래스의 메서드에서 값의 수정이 있지 않으면 const 를 추가합니다.
    ```
    class Object
    {
        ...
        void DoSomething(...) const;
        ...
    }
    ```
- 클래스는 독립된 소스 파일에 존재해야 하고, 소스 파일의 이름은 클래스와 일치해야 합니다.
    ```
    // Object.h
    class Object
    {
        ...
    }
    ```
- 클래스 메서드의 파라미터에 출력이 있으면 접두사로 out을 붙입니다.
    ```
    void GetSize(int& outWidth, int& outHeight);
    ```
- 정수 자료형을 선언할 때는 크기가 표시된 자료형을 사용합니다.
    ```
    uint8_t* buffer = nullptr;
    int32_t width = 1000;
    int32_t height = 800;
    ```
<br><br>


## 참고
- [언리얼 코딩 표준](https://docs.unrealengine.com/4.27/ko/ProductionPipelines/DevelopmentSetup/CodingStandard/)
- [MSDN : C# 코딩 규칙](https://learn.microsoft.com/ko-kr/dotnet/csharp/fundamentals/coding-style/coding-conventions)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [C++ 코딩 표준](https://docs.popekim.com/ko/coding-standards/cpp)