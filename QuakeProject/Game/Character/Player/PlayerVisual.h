/**
 * @file PlayerVisual.h
 * @brief プレイヤーの見た目を担当するクラス
 * @author suzuki.shunsuke
 * @date 2022/02/14
 */
#pragma once
#include <Model.h>
#include "Game/ScreenEffect/AfterGlow.h"
#include "../CharacterVisual.h"
#include "CaterpillarShaft.h"

 /**
 * @brief 戦車の各モデルの情報群
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
* @brief プレイヤーの見た目を担当するクラス
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

	/** 戦車のモデルたち */
	TankData mTankModel[eParts_Name::PARTS_NUM];

	/** 戦車モデル用コモンステート */
	std::unique_ptr<DirectX::CommonStates> mCommonState;

	/** 地面法線補完値 */
	DirectX::SimpleMath::Vector3 mNormal;

	/** 方位角保存値 */
	float mAzimuth;

	/** 仰角保存値 */
	float mElevationAngle;

	/** 左キャタピラ */
	std::unique_ptr<CaterpillarShaft> mpCaterLeft;
	/** 右キャタピラ */
	std::unique_ptr<CaterpillarShaft> mpCaterRight;
public:

	/**
	* @brief コンストラクタ
	*/
	PlayerVisual(Player* pOwner);

	/**
	* @brief デストラクタ
	*/
	~PlayerVisual();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

private:

	/**
	* @brief モデル達のワールド行列計算
	*/
	void CalculateWorldMatrixPersonal();


	/**
	* @brief モデル生成
	*/
	void CreateModel();

public:
	/**
	 * @brief	各パーツのワールド行列を取得する
	 * @param[in] type	パーツの種類
	 * @return  各パーツのワールド行列
	 */
	const DirectX::SimpleMath::Matrix& GetPartsMatrix(eParts_Name type) {
		return mTankModel[type].world;
	}
};