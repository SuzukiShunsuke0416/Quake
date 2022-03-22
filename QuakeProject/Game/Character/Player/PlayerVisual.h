/**
 * @file PlayerVisual.h
 * @brief �v���C���[�̌����ڂ�S������N���X
 * @author suzuki.shunsuke
 * @date 2022/02/14
 */
#pragma once
#include <Model.h>
#include "Game/ScreenEffect/AfterGlow.h"
#include "../CharacterVisual.h"
#include "CaterpillarShaft.h"

 /**
 * @brief ��Ԃ̊e���f���̏��Q
 */
struct TankData {
	std::unique_ptr<DirectX::Model> model;
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Vector3 ownDis;

	TankData()
		:model()
		, world()
		, ownDis()
	{
	};
};

class Player;
/**
* @brief �v���C���[�̌����ڂ�S������N���X
*/
class PlayerVisual :public CharacterVisual<Player>{
public:
	enum eParts_Name {
		eBase,
		eRide,
		eCannon,

		PARTS_NUM
	};

private:

	/** ��Ԃ̃��f������ */
	TankData mTankModel[eParts_Name::PARTS_NUM];

	/** ��ԃ��f���p�R�����X�e�[�g */
	std::unique_ptr<DirectX::CommonStates> mCommonState;

	/** �n�ʖ@���⊮�l */
	DirectX::SimpleMath::Vector3 mNormal;

	/** ���ʊp�ۑ��l */
	float mAzimuth;

	/** �p�ۑ��l */
	float mElevationAngle;

	/** ���L���^�s�� */
	std::unique_ptr<CaterpillarShaft> mpCaterLeft;
	/** �E�L���^�s�� */
	std::unique_ptr<CaterpillarShaft> mpCaterRight;
public:

	/**
	* @brief �R���X�g���N�^
	*/
	PlayerVisual(Player* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~PlayerVisual();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

private:

	/**
	* @brief ���f���B�̃��[���h�s��v�Z
	*/
	void CalculateWorldMatrixPersonal();


	/**
	* @brief ���f������
	*/
	void CreateModel();

public:
	/**
	 * @brief	�e�p�[�c�̃��[���h�s����擾����
	 * @param[in] type	�p�[�c�̎��
	 * @return  �e�p�[�c�̃��[���h�s��
	 */
	const DirectX::SimpleMath::Matrix& GetPartsMatrix(eParts_Name type) {
		return mTankModel[type].world;
	}
};