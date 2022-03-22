/**
 * @file Caterpillar.h
 * @brief キャタピラパーツクラス
 * @author suzuki.shunsuke
 * @date ???
 */
#pragma once

class CaterpillarShaft;

 /**
  * @brief キャタピラパーツクラス
  * @note 面情報ではなく、繋ぎ目部分(点)のデータ群です。
  */
class CaterpillarParts {
private:
	/** 角度 */
	float mAngle;

	/** 座標 */
	DirectX::SimpleMath::Vector3 mPos;

	/** 持ち主ポインタ */
	CaterpillarShaft* mpOwner;
public:
	/**
	* @brief コンストラクタ
	*/
	CaterpillarParts(CaterpillarShaft* pOwner);

	/**
	* @brief デストラクタ
	*/
	~CaterpillarParts();

	/**
	* @brief 更新
	*/
	void Update(float plusAng);

	/**
	* @brief 角度情報の設定
	*/
	void SetAngle(float ang) {
		mAngle = ang;
	}

	/**
	* @brief 座標の取得
	*/
	const DirectX::SimpleMath::Vector3& GetPos() {
		return mPos;
	}
};