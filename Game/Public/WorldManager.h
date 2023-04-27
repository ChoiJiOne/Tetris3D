#pragma once

#include <memory>
#include <map>

#include "Macro.h"

class GameObject;


/**
 * @brief 게임 월드 내 오브젝트를 관리합니다.
 * 
 * @note 이 클래스는 싱글턴 클래스로 헤더만 추가하면 바로 사용할 수 있습니다.
 */
class WorldManager
{
public:
	/**
	 * @brief 게임 오브젝트를 우선순위 기준으로 정렬하기 위한 객체입니다.
	 */
	struct CompareGameObjectOrder
	{
		/**
		 * @brief 두 피연산자를 비교하여 우선순위를 비교합니다.
		 * 
		 * @return 왼쪽 피연산자의 우선순위가 더 크면 true, 그렇지 않으면 false를 반환합니다.
		 */
		bool operator()(
			const std::pair<std::string, std::unique_ptr<GameObject>>& lhs,
			const std::pair<std::string, std::unique_ptr<GameObject>>& rhs
			)
		{
			return lhs.second->GetUpdateOrder() < rhs.second->GetUpdateOrder();
		}
	};

public:
	/**
	 * @biref 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(WorldManager);


	/**
	 * @brief WorldManager의 인스턴스를 얻습니다.
	 *
	 * @return WorldManager의 인스턴스 참조자를 반환합니다.
	 */
	static WorldManager& Get()
	{
		static WorldManager instance;
		return instance;
	}


	/**
	 * @brief WorldManager를 명시적으로 초기화합니다.
	 *
	 * @note 초기화 했거나 초기화 이후 Cleanup이 호출되지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Setup();


	/**
	 * @brief WorldManager의 내부 요소를 명시적으로 정리합니다.
	 *
	 * @note 초기화한 적이 없거나 Cleanup 호출 이후에 Setup을 호출하지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Cleanup();


	/**
	 * @brief 오브젝트 관리자에 게임 오브젝트를 추가합니다.
	 * 
	 * @param signature 게임 오브젝트의 시그니처 값입니다.
	 * @param gameObject 추가할 게임 오브젝트입니다.
	 * 
	 * @throws 시그니처 값에 대응하는 게임 오브젝트가 이미 존재하면 C++ 표준 예외를 던집니다.
	 */
	void AddGameObject(const std::string& signature, std::unique_ptr<GameObject> gameObject);


	/**
	 * @brief 관리 중인 게임 오브젝트를 얻습니다.
	 * 
	 * @param signature 게임 오브젝트의 시그니처 값입니다.
	 * 
	 * @return 시그니처 값에 대응하는 게임 오브젝트가 존재하면 포인터를 반환, 그렇지 않으면 nullptr를 반환합니다.
	 */
	GameObject* GetGameObject(const std::string& signature);


	/**
	 * @brief 관리 중인 게임 오브젝트를 삭제합니다.
	 * 
	 * @param signature 삭제할 게임 오브젝트의 시그니처 값입니다.
	 */
	void RemoveGameObject(const std::string& signature);


	/**
	 * @brief 관리 중인 게임 오브젝트를 우선 순위 기반으로 일괄 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	void Tick(float deltaSeconds);


private:
	/**
	 * @brief 게임 월드 내 오브젝트를 관리하는 클래스의 생성자입니다.
	 *
	 * @note 이 생성자는 아무 동작도 수행하지 않습니다.
	 */
	WorldManager() = default;


	/**
	 * @brief 게임 월드 내 오브젝트를 관리하는 클래스의 가상 소멸자입니다.
	 */
	virtual ~WorldManager();


private:
	/**
	 * @brief 초기화된 적이 있는지 확인합니다.
	 */
	bool bIsSetup_ = false;


	/**
	 * @brief 우선 순위 기준으로 정렬된 게임 오브젝트들입니다.
	 */
	std::map<std::string, std::unique_ptr<GameObject>, CompareGameObjectOrder> gameObjects_;
};