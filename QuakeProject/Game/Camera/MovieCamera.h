/**
 * @file MovieCamera.h
 * @brief OwnerActor���S�ɁA���܂�������������J����
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "MyCamera.h"

 /**
 * @brief OwnerActor���S�ɁA���܂�������������J����
 */
class MovieCamera :public MyCamera {
private:

public:
	/**
	* @brief �R���X�g���N�^
	*/
	MovieCamera(Actor* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~MovieCamera();

	/**
	* @brief �X�V
	*/
	void Update() override;

};