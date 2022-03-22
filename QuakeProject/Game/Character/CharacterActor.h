/**
 * @file CharacterActor.h
 * @brief �L�����N�^�[�n�A�N�^�[
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#pragma once
#include "Game/Actor/Actor.h"

 /**
 * @brief �L�����N�^�[�̃t���O�̎�ނ��܂Ƃ߂����O���
 */
namespace CharacterFlagBitMask{
	static unsigned IsOnTheGround = 1 << 0;
	static unsigned IsSlip = 1 << 1;
};

/**
* @brief �L�����N�^�[�n�A�N�^�[
*/
class CharacterActor :public Actor {
protected:
	/** ���x�ϐ� */
	DirectX::SimpleMath::Vector3 mVelocity;

	/** �ō����x�ϐ� */
	float mMaxSpeed;

	/** ������ */
	float mSpeedDownRat;

	/** �d�� */
	float mGravity;

	/** �W�����v�� */
	float mJumpPower;

	/** �L�����N�^�[�̃t���O�Q */
	unsigned mCharacterFlagBit;

	/** 1f�O�̍��W */
	DirectX::SimpleMath::Vector3 mLocationLog;

	/** 1f�O����̈ړ��� */
	DirectX::SimpleMath::Vector3 mDistanceFrommBefore1f;


public:
	/**
	* @brief �R���X�g���N�^
	*/
	CharacterActor() 
		:Actor()
		,mVelocity()
		,mMaxSpeed(0.8f)
		,mSpeedDownRat(0.95f)
		,mGravity(0.02f)
		,mJumpPower(1.0f)
		,mCharacterFlagBit(0U)
		,mLocationLog()
		,mDistanceFrommBefore1f()
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~CharacterActor() = default;

	/**
	* @brief �X�V
	*/
	virtual void Update() override;

	/**
	* @brief �`��
	*/
	virtual void Render() override = 0;

public:

	/**
	 * @brief	���x�ϐ��̎擾
	 * @return mVelocity ���x
	 */
	inline const DirectX::SimpleMath::Vector3& GetVelocity() {
		return mVelocity;
	}

	/**
	 * @brief	1f�O����̈ړ��ʂ̎擾
	 */
	inline const DirectX::SimpleMath::Vector3& GetDistanceFrommBefore1f() {
		return mDistanceFrommBefore1f;
	}

	/**
	 * @brief	1f�O����̈ړ���(�O�㍶�E)�̎擾
	 */
	inline DirectX::SimpleMath::Vector2 GetDistanceFrommBefore1fFromSky() {
		return DirectX::SimpleMath::Vector2(mDistanceFrommBefore1f.x, mDistanceFrommBefore1f.z);
	}

	/**
	* @brief ���x���ߖh�~
	*/
	inline void FitSpeed() 
	{
		if (mVelocity.Length() > mMaxSpeed) {
			mVelocity.Normalize();
			mVelocity *= mMaxSpeed;
		}
	}

	/**
	 * @brief	���x�ϐ��̐ݒ�
	 * @param[in] line	�V�������x
	 */
	inline void SetVelocity(const DirectX::SimpleMath::Vector3& vel) {
		mVelocity = vel;

		this->FitSpeed();
	}

	/**
	 * @brief	���x�����X�ɕύX����
	 * @param[in] line	�V�������x
	 * @note	�i�󂯎�����l���\���̈�ɂ��đ����j
	 */
	inline void AddForce(const DirectX::SimpleMath::Vector3& vel) {
		mVelocity += vel * 0.1f;

		this->FitSpeed();
	}

	/**
	* @brief �W�����v
	*/
	inline void Jump() {
		mVelocity.y = mJumpPower;

		this->FitSpeed();
	}

	/**
	* @brief ���n�����u�ԌĂ΂��C�x���g
	*/
	virtual void OnTouchGroundEvent() {};

public:
	/*===============================================================
			�t���O�n�̃A�N�Z�T
	===============================================================*/
	/**
	 * @brief	�t���O�ݒ�
	 * @param[in] mask	�ݒ肷��t���O�̎�ށiCharacterFlagBitMask�j
	 * @param[in] flag	�V�����t���O
	 */
	inline void SetFlag(unsigned mask, bool flag) {
		if (flag) {
			mCharacterFlagBit |= mask;
		}
		else {
			mCharacterFlagBit &= ~mask;
		}
	}
	/**
	 * @brief	�t���O�擾
	 */
	inline bool GetFlag(unsigned mask) {
		return mCharacterFlagBit & mask;
	}

	/**
	 * @brief	���n�t���O�ݒ�
	 * @param[in] flag	�V�����t���O
	 */
	inline void SetOnTheGroundFlag(bool flag) {
		if (flag) {
			mCharacterFlagBit |= CharacterFlagBitMask::IsOnTheGround;
		}
		else {
			mCharacterFlagBit &= ~CharacterFlagBitMask::IsOnTheGround;
		}
	}
	
	/**
	 * @brief	���n�t���O�擾
	 */
	inline bool NowIsOnTheGround() {
		return mCharacterFlagBit & CharacterFlagBitMask::IsOnTheGround;
	}

	/**
	 * @brief	����t���O�ݒ�
	 * @param[in] flag	�V�����t���O
	 */
	inline void SetSlipFlag(bool flag) {
		if (flag) {
			mCharacterFlagBit |= CharacterFlagBitMask::IsSlip;
		}
		else {
			mCharacterFlagBit &= ~CharacterFlagBitMask::IsSlip;
		}
	}

	/**
	 * @brief	����t���O�擾
	 */
	inline bool GetSlipFlag() {
		return mCharacterFlagBit & CharacterFlagBitMask::IsSlip;
	}
};
