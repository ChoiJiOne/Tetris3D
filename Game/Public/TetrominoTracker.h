#pragma once

#include "Tetromino.h"


/**
 * @brief 테트로미노 객체를 추적하는 오브젝트입니다.
 */
class TetrominoTracker : public GameObject
{
public:
	/**
	 * @brief 테트로미노 객체를 추적하는 오브젝트의 생성자입니다.
	 * 
	 * @param updateOrder 테트로미노 추적 객체의 업데이트 순위입니다.
	 * @param bIsActive 테트로미노 추적 객체의 활성화 여부입니다.
	 * @param tetrominoUpdateOrder 테트로미노의 업데이트 순위입니다.
	 * @param blockSize 테트로미노의 블럭 크기입니다.
	 * @param tetrominoMaxAccumulatedTime 테트로미노의 최대 누적 시간입니다.
	 * @param startPosition 테트로미노가 보드에 배치되어 시작되는 위치입니다.
	 * @param waitPosition 테트로미노가 대기하는 위치입니다.
	 */
	TetrominoTracker(
		int32_t updateOrder,
		bool bIsActive,
		int32_t tetrominoUpdateOrder,
		float blockSize,
		float tetrominoMaxAccumulatedTime,
		const DirectX::XMFLOAT3& startPosition,
		const DirectX::XMFLOAT3& waitPosition
	);


	/**
	 * @brief 테트로미노 추적 객체의 가상 소멸자입니다.
	 */
	virtual ~TetrominoTracker();


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(TetrominoTracker);


	/**
	 * @brief 테트로미노 추적 객체를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 현재 테트로미노 추적 객체가 추적 중인 테트로미노의 ID를 얻습니다.
	 * 
	 * @return 현재 테트로미노 추적 객체가 추적 중인 테트로미노의 ID를 반환합니다.
	 */
	int32_t GetCurrentTetrominoID() const { return currentTetrominoID_; }


	/**
	 * @brief 현재 테트로미노의 최대 누적 시간 값을 얻습니다.
	 * 
	 * @return 현재 테트로미노의 최대 누적 시간 값을 반환합니다.
	 */
	float GetTetrominoMaxAccumulatedTime() const { return tetrominoMaxAccumulatedTime_; }


	/**
	 * @brief 현재 테트로미노의 최대 누적 시간을 설정합니다.
	 * 
	 * @param tetrominoMaxAccumulatedTime 설정할 테트로미노의 최대 누적 시간입니다.
	 */
	void SetTetrominoMaxAccumulatedTime(float tetrominoMaxAccumulatedTime) {
		tetrominoMaxAccumulatedTime_ = tetrominoMaxAccumulatedTime;
	}


private:
	/**
	 * @brief 임의의 테트로미노 모양을 생성합니다.
	 * 
	 * @return 임의로 생성된 테트로미노 모양을 반환합니다.
	 */
	Tetromino::EShape GenerateRandomTetrominoShape() const;


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

	
private:
	/**
	 * @brief 테트로미노의 업데이트 순위입니다.
	 */
	int32_t tetrominoUpdateOrder_ = 0;


	/**
	 * @brief 테트로미노의 블록 크기입니다.
	 */
	float blockSize_ = 0.0f;


	/**
	 * @brief 테트로미노의 최대 누적 시간입니다.
	 */
	float tetrominoMaxAccumulatedTime_ = 0.0f;


	/**
	 * @brief 보드 상에서 테트로미노의 시작 위치입니다.
	 */
	DirectX::XMFLOAT3 startPosition_;


	/**
	 * @brief 테트로미노가 대기하는 위치입니다.
	 */
	DirectX::XMFLOAT3 waitPosition_;


	/**
	 * @brief 현재 테트로미노 추적 객체가 추적 중인 테트로미노의 ID입니다.
	 */
	int32_t currentTetrominoID_ = -1;
};