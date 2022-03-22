/**
 * @file CharacterActor.h
 * @brief キャラクター系アクター
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#pragma once
#include "Game/Actor/Actor.h"

 /**
 * @brief キャラクターのフラグの種類をまとめた名前空間
 */
namespace CharacterFlagBitMask{
	static unsigned IsOnTheGround = 1 << 0;
	static unsigned IsSlip = 1 << 1;
};

/**
* @brief キャラクター系アクター
*/
class CharacterActor :public Actor {
protected:
	/** 速度変数 */
	DirectX::SimpleMath::Vector3 mVelocity;

	/** 最高速度変数 */
	float mMaxSpeed;

	/** 減速率 */
	float mSpeedDownRat;

	/** 重力 */
	float mGravity;

	/** ジャンプ力 */
	float mJumpPower;

	/** キャラクターのフラグ群 */
	unsigned mCharacterFlagBit;

	/** 1f前の座標 */
	DirectX::SimpleMath::Vector3 mLocationLog;

	/** 1f前からの移動量 */
	DirectX::SimpleMath::Vector3 mDistanceFrommBefore1f;


public:
	/**
	* @brief コンストラクタ
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
	* @brief デストラクタ
	*/
	virtual ~CharacterActor() = default;

	/**
	* @brief 更新
	*/
	virtual void Update() override;

	/**
	* @brief 描画
	*/
	virtual void Render() override = 0;

public:

	/**
	 * @brief	速度変数の取得
	 * @return mVelocity 速度
	 */
	inline const DirectX::SimpleMath::Vector3& GetVelocity() {
		return mVelocity;
	}

	/**
	 * @brief	1f前からの移動量の取得
	 */
	inline const DirectX::SimpleMath::Vector3& GetDistanceFrommBefore1f() {
		return mDistanceFrommBefore1f;
	}

	/**
	 * @brief	1f前からの移動量(前後左右)の取得
	 */
	inline DirectX::SimpleMath::Vector2 GetDistanceFrommBefore1fFromSky() {
		return DirectX::SimpleMath::Vector2(mDistanceFrommBefore1f.x, mDistanceFrommBefore1f.z);
	}

	/**
	* @brief 速度超過防止
	*/
	inline void FitSpeed() 
	{
		if (mVelocity.Length() > mMaxSpeed) {
			mVelocity.Normalize();
			mVelocity *= mMaxSpeed;
		}
	}

	/**
	 * @brief	速度変数の設定
	 * @param[in] line	新しい速度
	 */
	inline void SetVelocity(const DirectX::SimpleMath::Vector3& vel) {
		mVelocity = vel;

		this->FitSpeed();
	}

	/**
	 * @brief	速度を徐々に変更する
	 * @param[in] line	新しい速度
	 * @note	（受け取った値を十分の一にして足す）
	 */
	inline void AddForce(const DirectX::SimpleMath::Vector3& vel) {
		mVelocity += vel * 0.1f;

		this->FitSpeed();
	}

	/**
	* @brief ジャンプ
	*/
	inline void Jump() {
		mVelocity.y = mJumpPower;

		this->FitSpeed();
	}

	/**
	* @brief 着地した瞬間呼ばれるイベント
	*/
	virtual void OnTouchGroundEvent() {};

public:
	/*===============================================================
			フラグ系のアクセサ
	===============================================================*/
	/**
	 * @brief	フラグ設定
	 * @param[in] mask	設定するフラグの種類（CharacterFlagBitMask）
	 * @param[in] flag	新しいフラグ
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
	 * @brief	フラグ取得
	 */
	inline bool GetFlag(unsigned mask) {
		return mCharacterFlagBit & mask;
	}

	/**
	 * @brief	着地フラグ設定
	 * @param[in] flag	新しいフラグ
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
	 * @brief	着地フラグ取得
	 */
	inline bool NowIsOnTheGround() {
		return mCharacterFlagBit & CharacterFlagBitMask::IsOnTheGround;
	}

	/**
	 * @brief	滑りフラグ設定
	 * @param[in] flag	新しいフラグ
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
	 * @brief	滑りフラグ取得
	 */
	inline bool GetSlipFlag() {
		return mCharacterFlagBit & CharacterFlagBitMask::IsSlip;
	}
};
