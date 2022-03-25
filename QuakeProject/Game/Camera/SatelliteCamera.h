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

	/** �J�[�\�����w���Ă���ꏊ */
	DirectX::SimpleMath::Vector3 mChoosingPoint;

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
	void Update() override;

private:
	/**
	* @brief �r���[�s��v�Z�֐����Ă�
	*/
	void CallCalculateViewMatrixFunction();

	/**
	* @brief �J�[�\�����w���Ă���ꏊ���v�Z����
	*/
	void CalculateChoosingPoint();

	/**
	* @brief �n�ʂƂ̋��������v�Z����
	*/
	void CalculateDistance();

public:

	/**
	* @brief �J�[�\�����w���Ă���ꏊ���擾����
	*/
	inline const DirectX::SimpleMath::Vector3& 
	GetChoosingPoint() { return mChoosingPoint; }

	/**
	* @brief �^�[�Q�b�g���W�����炩�ɕύX����
	*/
	void SetTargetPosRubber(const DirectX::SimpleMath::Vector3& goal);

	/**
	* @brief �^�[�Q�b�g���W�����炩�Ƀf�t�H���g�l�ɂ���
	*/
	void SetTargetPosDefaultRubber();

	/**
	* @brief �^�[�Q�b�g���W���f�t�H���g�l�ɂ���
	*/
	void SetTargetPosDefault();
};


/**
* @brief �T�e���C�g�J�������������̃A�N�^�[
*/
class SatelliteCameraUser :public Actor {
private:

	/** �T�e���C�g�J���� */
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
	* @note �T�e���C�g�J�����̃A�v�f��
			�J�����}�l�[�W�����S�����Ă���
	*/
	inline void Update() override {}

	/**
	* @brief �`��
	*/
	void Render() override {}

public:
	/**
	* @brief �J�����̎擾
	*/
	inline SatelliteCamera* GetCamera() {
		return mpCamera;
	}
};