/**
 * @file	StageTriangle.h
 * @brief	�I�[�i�[�̑����ɂ���n�ʏ��i�O�p�j
			���܂Ƃ߂��R���|�[�l���g
 * @author	suzuki.shunsuke
 * @date	2022/02/02
 */
#pragma once

#include "Game/Component/Component.h"
#include "Libraries/MyLibraries/Utility.h"

class StagePoint;

/**
* @brief �I�[�i�[�̑����ɂ���n�ʏ��i�O�p�j
*/
class StageTriangle :public Component {
public:
	/** �Ǘ�����|�C���g�̐� */
	static const int POINT_SIZE = 3;
private:
	/** �I�[�i�[�̑����ɂ���X�e�[�W�|�C���g */
	StagePoint* mpStagePoint[POINT_SIZE];

	/** �ʂ̖@���x�N�g�� */
	DirectX::SimpleMath::Vector3 mNormal;

	/** �@�������ւ̉�]�s�� */
	DirectX::SimpleMath::Matrix mNormalRotationMatrix;

	/** ���ʊp */
	float mAzimuth;

	/** �p */
	float mElevationAngle;
public:
	/**
	 * @brief	������
	 * @param[in] pOwner ������
	 */
	StageTriangle(Actor* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~StageTriangle();

	/**
	* @brief �X�V
	* @note (StageManager����Ă΂��)
	*/
	void Update();
public:

	/**
	* @brief �����X�e�[�W�|�C���g�z��̐擪�A�h���X���擾
	*/
	inline StagePoint** GetStagePoints() {
		return mpStagePoint;
	}

	/**
	* @brief �����X�e�[�W�|�C���g���擾
	*/
	inline StagePoint* GetStagePoints(int element) {
		return mpStagePoint[element];
	}

	/**
	* @brief �@���x�N�g���擾
	*/
	inline const DirectX::SimpleMath::Vector3& GetNormal() {
		return mNormal;
	}

	/**
	* @brief �@�������̉�]�s����擾
	*/
	inline const DirectX::SimpleMath::Matrix& GetNormalRotMat() {
		return mNormalRotationMatrix;
	}

	/**
	* @brief ���ʊp���擾
	*/
	inline float GetAzimuth() {
		return mAzimuth;
	}

	/**
	* @brief �p���擾
	*/
	inline float GetElevationAngle() {
		return mElevationAngle;
	}

};