#pragma once

#include <directxmath.h>

#include "GameTimer.h"
#include "Scene.h"

class Tetromino;
class Board;


/**
 * @brief 게임 플레이 씬입니다.
 */
class PlayScene : public Scene
{
public:
	/**
	 * @brief 게임 플레이 씬의 생성자입니다.
	 * 
	 * @param updateOrder 게임 플레이 씬의 업데이트 순위입니다.
	 * @param bIsActive 게임 플레이 씬의 활성화 여부입니다.
	 */
	PlayScene(int32_t updateOrder, bool bIsActive)
		: Scene(updateOrder, bIsActive) {}


	/**
	 * @brief 게임 플레이 씬의 가상 소멸자입니다.
	 */
	virtual ~PlayScene() {}


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(PlayScene);
	

	/**
	 * @brief 게임 플레이 씬을 업데이트합니다.
	 *
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 게임 플레이 씬에 진입합니다.
	 */
	virtual void Entry() override;


	/**
	 * @brief 게임 플레이 씬으로부터 나갑니다.
	 */
	virtual void Leave() override;


	/**
	 * @brief 게임 플레이 시간을 얻습니다.
	 * 
	 * @return 초단위 게임 플레이 시간 값을 반환합니다.
	 */
	float GetPlayTime() const { return timer_.GetTotalSeconds(); }


	/**
	 * @brief 테트로미노의 최대 누적 시간을 설정합니다.
	 * 
	 * @note 이때, 시간 단위는 초단위입니다.
	 * 
	 * @param tetrominoMaxAccumulatedTime 설정할 테트로미노의 최대 누적 시간입니다.
	 */
	void SetTetrominoMaxAccumulatedTime(float tetrominoMaxAccumulatedTime)
	{
		tetrominoMaxAccumulatedTime_ = tetrominoMaxAccumulatedTime;
	}


private:
	/**
	 * @brief 테트로미노를 생성합니다.
	 *
	 * @param tetrominoID 테트로미노의 ID입니다.
	 * @param position 월드 상 테트로미노의 기준 위치입니다.
	 */
	void GenerateTetromino(int32_t tetrominoID, const DirectX::XMFLOAT3& position);


	/**
	 * @brief ID에 대응하는 테트로미노를 얻습니다.
	 *
	 * @param tetrominoID 테트로미노의 ID입니다.
	 *
	 * @return ID에 대응하는 테트로미노의 포인터 값을 반환합니다.
	 */
	Tetromino* GetTetromino(int32_t tetrominoID);


	/**
	 * @brief ID에 대응하는 테트로미노를 삭제합니다.
	 *
	 * @param tetrominoID 테트로미노의 ID입니다.
	 */
	void DestroyTetromino(int32_t tetrominoID);


	/**
	 * @brief 플레이 씬의 테트리스 보드를 얻습니다.
	 * 
	 * @return 플레이 씬 테트리스 보드의 포인터를 반환합니다.
	 */
	Board* GetBoard();


	/**
	 * @brief 테트리스 게임 요소를 초기화합니다.
	 */
	void SetupTetrisProperties();


	/**
	 * @brief 테트리스 게임 오브젝트를 초기화합니다.
	 */
	void SetupTetrisObject();


	/**
	 * @brief 테트리스 게임 UI를 초기화합니다.
	 */
	void SetupTetrisUI();


	/**
	 * @brief 테트리스 게임 요소의 초기화를 수행합니다.
	 */
	void CleanupTetrisResource();


	/**
	 * @brief 전체 게임 요소들을 중지합니다.
	 */
	void StopTetrisAllObject();

	
private:
	/**
	 * @brief 테트로미노의 업데이트 순위입니다.
	 */
	int32_t tetrominoUpdateOrder_ = 0;


	/**
	 * @brief 보드의 업데이트 순위입니다.
	 */
	int32_t boardUpdateOrder_ = 0;


	/**
	 * @brief 테트로미노의 블록 크기입니다.
	 */
	float blockSize_ = 0.0f;


	/**
	 * @brief 테트로미노의 최대 누적 시간입니다.
	 */
	float tetrominoMaxAccumulatedTime_ = 0.0f;


	/**
	 * @brief 보드의 세로 크기입니다.
	 */
	int32_t boardRowCount_ = 0;


	/**
	 * @brief 보드의 가로 크기입니다.
	 */
	int32_t boardColCount_ = 0;


	/**
	 * @brief 보드의 기준 좌표입니다.
	 */
	DirectX::XMFLOAT3 boardBasePosition_;


	/**
	 * @brief 보드 상에서 테트로미노의 시작 위치입니다.
	 */
	DirectX::XMFLOAT3 tetrominoStartPosition_;


	/**
	 * @brief 테트로미노가 대기하는 위치입니다.
	 */
	DirectX::XMFLOAT3 tetrominoWaitPosition_;


	/**
	 * @brief 현재 테트로미노 추적 객체가 추적 중인 테트로미노의 ID입니다.
	 */
	int32_t currentTetrominoID_ = 0;


	/**
	 * @brief 게임 내 UI의 업데이트 순위입니다.
	 */
	int32_t uiUpdateOrder_ = 0;


	/**
	 * @brief 게임 플레이 타이머입니다.
	 */
	GameTimer timer_;
};