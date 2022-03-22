/**
 * @file MyCamera.h
 * @brief ��{�I�ȃJ�����N���X�B�p�r�ɍ��킹�Čp���B
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Camera.h"
#include "Game/Component/Component.h"

 /**
 * @brief	��{�I�ȃJ�����N���X�B
 * @note	�p�r�ɍ��킹�Čp���B
 */
class MyCamera
	: public Camera
	, public Component
{
private:


public:
	/**
	* @brief �R���X�g���N�^
	*/
	MyCamera(Actor* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~MyCamera();

	/**
	* @brief �X�V
	*/
	virtual void Update() override{}
};