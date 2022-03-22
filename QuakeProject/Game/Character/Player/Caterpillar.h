/**
 * @file Caterpillar.h
 * @brief �L���^�s���p�[�c�N���X
 * @author suzuki.shunsuke
 * @date ???
 */
#pragma once

class CaterpillarShaft;

 /**
  * @brief �L���^�s���p�[�c�N���X
  * @note �ʏ��ł͂Ȃ��A�q���ڕ���(�_)�̃f�[�^�Q�ł��B
  */
class CaterpillarParts {
private:
	/** �p�x */
	float mAngle;

	/** ���W */
	DirectX::SimpleMath::Vector3 mPos;

	/** ������|�C���^ */
	CaterpillarShaft* mpOwner;
public:
	/**
	* @brief �R���X�g���N�^
	*/
	CaterpillarParts(CaterpillarShaft* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~CaterpillarParts();

	/**
	* @brief �X�V
	*/
	void Update(float plusAng);

	/**
	* @brief �p�x���̐ݒ�
	*/
	void SetAngle(float ang) {
		mAngle = ang;
	}

	/**
	* @brief ���W�̎擾
	*/
	const DirectX::SimpleMath::Vector3& GetPos() {
		return mPos;
	}
};