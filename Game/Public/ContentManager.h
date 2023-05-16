#pragma once

#include <memory>
#include <unordered_map>

#include "Macro.h"

class EffectShader;
class Texture2D;
class StaticMesh;
class TTFont;
class Sound;


/**
 * @brief 게임 내의 컨텐츠를 관리합니다.
 * 
 * @note 이 클래스는 싱글턴 클래스로 헤더만 추가하면 바로 사용할 수 있습니다.
 */
class ContentManager
{
public:
	/**
	 * @biref 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(ContentManager);


	/**
	 * @brief ContentManager의 인스턴스를 얻습니다.
	 * 
	 * @return ContentManager의 인스턴스 참조자를 반환합니다.
	 */
	static ContentManager& Get()
	{
		static ContentManager instance;
		return instance;
	}


	/**
	 * @brief ContentManager를 명시적으로 초기화합니다.
	 * 
	 * @note 초기화 했거나 초기화 이후 Cleanup이 호출되지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Setup();


	/**
	 * @brief ContentManager의 내부 요소를 명시적으로 정리합니다.
	 * 
	 * @note 초기화한 적이 없거나 Cleanup 호출 이후에 Setup을 호출하지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Cleanup();


	/**
	 * @brief 컨텐츠 관리자에 효과 셰이더를 추가합니다.
	 * 
	 * @param signature 효과 셰이더의 시그니처 값입니다.
	 * @param effectShader 관리자에 추가할 효과 셰이더입니다.
	 * 
	 * @throws 시그니처 값에 대응하는 효과 셰이더가 이미 존재하면 C++ 표준 예외를 던집니다.
	 */
	EffectShader* AddEffectShader(const std::string& signature, std::unique_ptr<EffectShader> effectShader);


	/**
	 * @brief 관리 중인 효과 셰이더를 얻습니다.
	 * 
	 * @param signature 효과 셰이더의 시그니처 값입니다.
	 * 
	 * @return 시그니처 값에 대응하는 효과 셰이더가 존재하면 포인터를 반환, 그렇지 않으면 nullptr 값을 반환합니다.
	 */
	EffectShader* GetEffectShader(const std::string& signature);


	/**
	 * @brief 관리 중인 효과 셰이더를 삭제합니다.
	 * 
	 * @note 시그니처 값에 대응하는 효과 셰이더가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 * 
	 * @param signature 효과 셰이더의 시그니처 값입니다.
	 */
	void RemoveEffectShader(const std::string& signature);


	/**
	 * @brief 컨텐츠 관리자에 텍스처를 추가합니다.
	 *
	 * @param signature 텍스처의 시그니처 값입니다.
	 * @param texture 관리자에 추가할 텍스처입니다.
	 *
	 * @throws 시그니처 값에 대응하는 텍스처가 이미 존재하면 C++ 표준 예외를 던집니다.
	 */
	Texture2D* AddTexture2D(const std::string& signature, std::unique_ptr<Texture2D> texture);


	/**
	 * @brief 관리 중인 텍스처를 얻습니다.
	 *
	 * @param signature 텍스처의 시그니처 값입니다.
	 *
	 * @return 시그니처 값에 대응하는 텍스처가 존재하면 포인터를 반환, 그렇지 않으면 nullptr 값을 반환합니다.
	 */
	Texture2D* GetTexture2D(const std::string& signature);


	/**
	 * @brief 관리 중인 텍스처를 삭제합니다.
	 *
	 * @note 시그니처 값에 대응하는 텍스처가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 *
	 * @param signature 텍스처의 시그니처 값입니다.
	 */
	void RemoveTexture2D(const std::string& signature);


	/**
	 * @brief 컨텐츠 관리자에 정적 메시를 추가합니다.
	 *
	 * @param signature 정적 메시의 시그니처 값입니다.
	 * @param staticMesh 관리자에 추가할 정적 메시입니다.
	 *
	 * @throws 시그니처 값에 대응하는 정적 메시가 이미 존재하면 C++ 표준 예외를 던집니다.
	 */
	StaticMesh* AddStaticMesh(const std::string& signature, std::unique_ptr<StaticMesh> staticMesh);


	/**
	 * @brief 관리 중인 정적 메시를 얻습니다.
	 *
	 * @param signature 정적 메시의 시그니처 값입니다.
	 *
	 * @return 시그니처 값에 대응하는 정적 메시가 존재하면 포인터를 반환, 그렇지 않으면 nullptr 값을 반환합니다.
	 */
	StaticMesh* GetStaticMesh(const std::string& signature);


	/**
	 * @brief 관리 중인 정적 메시를 삭제합니다.
	 *
	 * @note 시그니처 값에 대응하는 정적 메시가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 *
	 * @param signature 정적 메시의 시그니처 값입니다.
	 */
	void RemoveStaticMesh(const std::string& signature);


	/**
	 * @brief 컨텐츠 관리자에 트루 타입 폰트를 추가합니다.
	 * 
	 * @param signature 트루 타입 폰트의 시그니처 값입니다.
	 * @param font 관리자에 추가할 트루 타입 폰트입니다.
	 * 
	 * @throws 시그니처 값에 대응하는 트루 타입 폰트가 이미 존재하면 C++ 표준 예외를 던집니다.
	 */
	TTFont* AddTTFont(const std::string& signature, std::unique_ptr<TTFont> font);


	/**
	 * @brief 관리 중인 트루 타입 폰트를 얻습니다.
	 *
	 * @param signature 트루 타입 폰트의 시그니처 값입니다.
	 *
	 * @return 시그니처 값에 대응하는 트루 타입 폰트가 존재하면 포인터를 반환, 그렇지 않으면 nullptr 값을 반환합니다.
	 */
	TTFont* GetTTFont(const std::string& signature);


	/**
	 * @brief 관리 중인 트루 타입 폰트를 삭제합니다.
	 *
	 * @note 시그니처 값에 대응하는 트루 타입 폰트가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 *
	 * @param signature 트루 타입 폰트의 시그니처 값입니다.
	 */
	void RemoveTTFont(const std::string& signature);


	/**
	 * @brief 컨텐츠 관리자에 사운드를 추가합니다.
	 *
	 * @param signature 사운드의 시그니처 값입니다.
	 * @param sound 관리자에 추가할 사운드입니다.
	 *
	 * @throws 시그니처 값에 대응하는 사운드가 이미 존재하면 C++ 표준 예외를 던집니다.
	 */
	Sound* AddSound(const std::string& signature, std::unique_ptr<Sound> sound);


	/**
	 * @brief 관리 중인 사운드를 얻습니다.
	 *
	 * @param signature 사운드의 시그니처 값입니다.
	 *
	 * @return 시그니처 값에 대응하는 사운드가 존재하면 포인터를 반환, 그렇지 않으면 nullptr 값을 반환합니다.
	 */
	Sound* GetSound(const std::string& signature);


	/**
	 * @brief 관리 중인 사운드를 삭제합니다.
	 *
	 * @note 시그니처 값에 대응하는 사운드가 존재하지 않으면 아무 동작도 수행하지 않습니다.
	 *
	 * @param signature 사운드의 시그니처 값입니다.
	 */
	void RemoveSound(const std::string& signature);
	

private:
	/**
	 * @brief 게임 내의 컨텐츠를 관리하는 클래스의 생성자입니다.
	 * 
	 * @note 이 생성자는 아무 동작도 수행하지 않습니다.
	 */
	ContentManager();


	/**
	 * @brief 게임 내의 컨텐츠를 관리하는 클래스의 가상 소멸자입니다.
	 */
	virtual ~ContentManager();


private:
	/**
	 * @brief 초기화된 적이 있는지 확인합니다.
	 */
	bool bIsSetup_ = false;


	/**
	 * @brief 컨텐츠 매니저가 관리 중인 셰이더입니다.
	 */
	std::unordered_map<std::string, std::unique_ptr<EffectShader>> effectShaders_;


	/**
	 * @brief 컨텐츠 매니저가 관리 중인 정적 메시입니다.
	 */
	std::unordered_map<std::string, std::unique_ptr<StaticMesh>> staticMeshs_;


	/**
	 * @brief 컨텐츠 매니저가 관리 중인 텍스처입니다.
	 */
	std::unordered_map<std::string, std::unique_ptr<Texture2D>> textures_;


	/**
	 * @brief 컨텐츠 매니저가 관리 중인 트루 타입 폰트입니다.
	 */
	std::unordered_map<std::string, std::unique_ptr<TTFont>> fonts_;


	/**
	 * @brief 컨텐츠 매니저가 관리 중인 사운드 리소스입니다.
	 */
	std::unordered_map<std::string, std::unique_ptr<Sound>> sounds_;
};