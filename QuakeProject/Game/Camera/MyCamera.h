/**
 * @file MyCamera.h
 * @brief 基本的なカメラクラス。用途に合わせて継承。
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Camera.h"
#include "Game/Component/Component.h"

 /**
 * @brief	基本的なカメラクラス。
 * @note	用途に合わせて継承。
 */
class MyCamera
	: public Camera
	, public Component
{
private:


public:
	/**
	* @brief コンストラクタ
	*/
	MyCamera(Actor* pOwner);

	/**
	* @brief デストラクタ
	*/
	virtual ~MyCamera();

	/**
	* @brief 更新
	*/
	virtual void Update() override{}
};