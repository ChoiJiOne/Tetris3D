#pragma once

#include <d3d11.h>

#include "Macro.h"

class Window;


/**
 * @brief 렌더링 관련 처리를 수행합니다.
 * 
 * @note 이 클래스는 싱글턴 클래스로 헤더만 추가하면 바로 사용할 수 있습니다.
 */
class RenderManager
{
public:
	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(RenderManager);


	/**
	 * @brief RenderManager의 인스턴스를 얻습니다.
	 * 
	 * @return RenderManager의 인스턴스 참조자를 반환합니다.
	 */
	static RenderManager& Get()
	{
		static RenderManager instance;
		return instance;
	}


	/**
	 * @brief RenderManager를 명시적으로 초기화합니다.
	 * 
	 * @note 초기화 이후 Cleanup이 호출되지 않았다면 아무 동작도 수행하지 않습니다.
	 * 
	 * @param renderTargetWindow 렌더링 대상이 되는 윈도우입니다.
	 */
	void Setup(Window* renderTargetWindow);


	/**
	 * @brief RenderManager의 내부 요소를 명시적으로 정리합니다.
	 * 
	 * @note 초기화한 적이 없거나 Cleanup 호출 이후에 Setup을 호출하지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Cleanup();


	/**
	 * @brief 프레임 렌더링을 시작하고 백버퍼를 초기화합니다.
	 * 
	 * @param red 색상 버퍼의 초기화 할 색상 중 R값입니다.
	 * @param green 색상 버퍼의 초기화 할 색상 중 G값입니다.
	 * @param blue 색상 버퍼의 초기화 할 색상 중 B값입니다.
	 * @param alpha 색상 버퍼의 초기화 할 색상 중 A값입니다.
	 * @param depth 깊이 버퍼의 초기화 할 값입니다. 기본 값은 1.0 입니다
	 * @param stencil 스텐실 버퍼의 초기화 할 값입니다. 기본 값은 0입니다.
	 */
	void BeginFrame(float red, float green, float blue, float alpha, float depth = 1.0f, uint8_t stencil = 0);


	/**
	 * @brief 프레임 렌더링을 종료하고 백버퍼와 프론트 버퍼를 스왑합니다.
	 * 
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-present
	 *
	 * @param bIsVSync 수직 동기화 여부를 확인합니다. 기본적으로 수직 동기화 처리합니다.
	 *
	 * @throws 백 버퍼와 프론트 버퍼의 교환에 실패하면 C++ 표준 예외를 던집니다.
	 */
	void EndFrame(bool bIsVSync = true);

	
private:
	/**
	 * @brief 렌더링 처리를 수행하는 클래스의 생성자입니다.
	 * 
	 * @note 이 생성자는 아무 동작도 수행하지 않습니다.
	 */
	RenderManager() = default;


	/**
	 * @brief 렌더링 처리를 수행하는 클래스의 가상 소멸자입니다.
	 */
	virtual ~RenderManager();

	
	/**
	 * @brief 디바이스 및 컨텍스트를 생성합니다.
	 * 
	 * @return 디바이스 및 컨텍스트의 생성 결과를 반환합니다. 성공했다면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateDeviceAndContext();


	/**
	 * @brief 스왑 체인을 생성합니다.
	 *
	 * @param WindowHandle 대상이 되는 윈도우 핸들입니다.
	 *
	 * @return 스왑 체인 생성 결과를 반환합니다. 성공했다면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateSwapChain(HWND windowHandle);


	/**
	 * @brief 렌더 타겟 뷰를 생성합니다.
	 *
	 * @return 렌더 타겟 뷰 생성 결과를 반환합니다. 성공했다면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateRenderTargetView();


	/**
	 * @brief 깊이 스텐실 뷰를 생성합니다.
	 *
	 * @return 깊이 스텐실 버퍼 생성 결과를 반환합니다. 성공했다면 S_OK, 그렇지 않다면 그 이외의 값을 반환합니다.
	 */
	HRESULT CreateDepthStencilView();


private:
	/**
	 * @brief 초기화된 적이 있는지 확인합니다.
	 */
	bool bIsSetup_ = false;


	/**
	 * @brief 렌더링 대상이 되는 윈도우입니다.
	 */
	Window* renderTargetWindow_ = nullptr;


	/**
	 * @brief 드라이버 타입입니다.
	 *
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
	 */
	D3D_DRIVER_TYPE	driverType_ = D3D_DRIVER_TYPE_NULL;


	/**
	 * @brief 디바이스가 대상으로 하는 기능입니다.
	 *
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_feature_level
	 */
	D3D_FEATURE_LEVEL featureLevel_ = D3D_FEATURE_LEVEL_11_0;


	/**
	 * @brief 리소스를 만들고 디스플레이 어댑터의 기능을 열거하는 데 사용합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/overviews-direct3d-11-devices-intro
	 */
	ID3D11Device* device_ = nullptr;


	/**
	 * @brief 디바이스가 소유한 리소스를 사용하여 파이프라인 상태를 설정하고 렌더링 명령을 생성하는 데 사용합니다.
	 *
	 * @see https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/overviews-direct3d-11-devices-intro
	 */
	ID3D11DeviceContext* context_ = nullptr;


	/**
	 * @brief 렌더링된 데이터를 출력에 표시하기 전에 저장합니다.
	 *
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nn-dxgi-idxgiswapchain
	 */
	IDXGISwapChain* swapChain_ = nullptr;


	/**
	 * @brief 렌더링 중 엑세스 할 수 있는 리소스입니다.
	 *
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11rendertargetview
	 */
	ID3D11RenderTargetView* renderTargetView_ = nullptr;


	/**
	 * @brief 깊이 스텐실 버퍼 리소스입니다.
	 *
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11texture2d
	 */
	ID3D11Texture2D* depthStencilBuffer_ = nullptr;


	/**
	 * @brief 깊이 스텐실 뷰 리소스입니다.
	 *
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11depthstencilview
	 */
	ID3D11DepthStencilView* depthStencilView_ = nullptr;
};