/**
 * @file CaterpillarShaft.h
 * @brief キャタピラクラス
 * @author suzuki.shunsuke
 * @date ???
 */
#pragma once
#include "Caterpillar.h"
#include <vector>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include "Libraries/MyLibraries/MyVertexTypes.h"
#include "Game/ScreenEffect/BrightChanger.h"

 /**
  * @brief キャタピラがどちら側にあるか
  */
enum class eCaterSideType {
	Left = -1,
	Right = 1,
};

class PlayerVisual;
/**
 * @brief キャタピラ初期化データ群
 */
struct Cater_Init_Desc {
	PlayerVisual* pOwner;
	eCaterSideType sideType;

	Cater_Init_Desc() 
		:pOwner()
		,sideType()
	{
	}
};

/**
 * @brief キャタピラクラス
 */
class CaterpillarShaft{
public:
	

private:
	/** 持ち主 */
	PlayerVisual* mpOwner;

	/** 回転速度 */
	float mRotVel;

	/** キャタピラたち */
	std::vector<std::unique_ptr<CaterpillarParts>> 
		mCaterpillars;

	/** キャタピラポリゴンの型 */
	using CaterpillarPolygonType = DataForPrimitiveBatchRender<DirectX::VertexPositionColor>;

	/** キャタピラ描画用データ群 */
	std::unique_ptr<CaterpillarPolygonType> 
		mpCaterpillarPolygonData;

	/** 左右情報 */
	eCaterSideType mSideType;

	/** 光らせ担当 */
	std::unique_ptr<BrightChanger> mpBright;

private: // 外部読み込み系

	/** キャタピラ(全体)の半径 */
	float mHalfSize;

	/** キャタピラの縦横比（横が縦の何倍か） */
	float mSideRat;

	/** キャタピラの数 */
	int mCaterNum;

	/** キャタピラ(面)の横幅 */
	float mHalfWidthSize;

	/** 戦車からの距離 */
	float mDistanceFromTank;

public:
	/**
	* @brief コンストラクタ
	* @param[in] cid Cater_Init_Desc 自クラス初期化用データ群
	*/
	CaterpillarShaft(const Cater_Init_Desc& cid);

	/**
	* @brief デストラクタ
	*/
	~CaterpillarShaft();

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
	* @brief 外部読み込み系データの初期化
	*/
	void InitExternalData();

public:
	/**
	* @brief 半径の取得
	*/
	inline float GetHalfSize() {
		return mHalfSize;
	}
	/**
	* @brief 縦横比の取得
	*/
	inline float GetSideRat() {
		return mSideRat;
	}
	/**
	* @brief キャタピラの数の取得
	*/
	inline int GetCaterNum() {
		return mCaterNum;
	}
};