/**
 * @file MovieCamera.h
 * @brief OwnerActor中心に、決まった動きをするカメラ
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "MyCamera.h"

 /**
 * @brief OwnerActor中心に、決まった動きをするカメラ
 */
class MovieCamera :public MyCamera {
private:

public:
	/**
	* @brief コンストラクタ
	*/
	MovieCamera(Actor* pOwner);

	/**
	* @brief デストラクタ
	*/
	~MovieCamera();

	/**
	* @brief 更新
	*/
	void Update() override;

};