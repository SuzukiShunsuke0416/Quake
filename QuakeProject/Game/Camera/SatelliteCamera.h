/**
 * @file SatelliteCamera.h
 * @brief �ォ�猩�����_�̃J�����B�q���J�����B
 * @author suzuki.shunsuke
 * @date 2022/03/23
 */
#pragma once
#include "MyCamera.h"
#include "Game/Actor/Actor.h"
#include "Libraries/MyLibraries/Utility.h"

 /**
 * @brief �ォ�猩�����_�̃J����
 */
class SatelliteCamera final:public MyCamera{
private:
	/** �n�ʂƂ̋��� */
	float mDistanceRat;

	/** �����_ */
	DirectX::SimpleMath::Vector3 mTargetPos;

	/** �J�������W�̃X�^�[�g�n�_ */
	DirectX::SimpleMath::Vector3 mEyeOriginPos;
	
	/** �}�E�X�z�C�[���̒l */
	Save<int> mWheelValue;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	SatelliteCamera(Actor* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~SatelliteCamera() = default;

	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	void Update();

private:
	/**
	* @brief �r���[�s��v�Z�֐����Ă�
	*/
	void CallCalculateViewMatrixFunction();

	/**
	* @brief �^�[�Q�b�g���W���v�Z����
	*/
	void CalculateTargetPos();

	/**
	* @brief �n�ʂƂ̋��������v�Z����
	*/
	void CalculateDistance();
};


/**
* @brief �T�e���C�g�J�������������̃A�N�^�[
*/
class SatelliteCameraUser :public Actor {
private:
	SatelliteCamera* mpCamera;
public:
	/**
	* @brief �R���X�g���N�^
	*/
	SatelliteCameraUser() 
		:Actor()
		,mpCamera()
	{
		mpCamera = new SatelliteCamera(this);
		// �폜�̓J�����}�l�[�W�����S��
	}

	/**
	* @brief �f�X�g���N�^
	*/
	~SatelliteCameraUser() = default;

	/**
	* @brief ������
	*/
	inline void Initialize() {
		mpCamera->Initialize();
	}

	/**
	* @brief �X�V
	*/
	inline void Update() override {
		mpCamera->Update();
	}

	/**
	* @brief �`��
	*/
	void Render() override {}
};