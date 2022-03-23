/**
 * @file StageManager.h
 * @brief ステージ情報を管理するクラス
 * @author suzuki.shunsuke
 * @date 2022/02/03
 */
#pragma once
#include <vector>
#include "StagePoint.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Game/UtilityManager/SingletonFounder.h"
#include "Libraries/MyLibraries/MyVertexTypes.h"
#include "StageTriangle.h"
#include <unordered_map>
#include "Game/Collision/SimpleCheckFunctions.h"
#include "Game/ScreenEffect/BrightChanger.h"

class StageTriangleManager;
/**
 * @brief ステージ情報を管理するクラス
 */
class StageManager :public SingletonFounder<StageManager> {
	friend SingletonFounder;
public:
	
private:
	/** ステージ点情報 */
	std::vector<std::vector<StagePoint*>> mpStagePoints;

	/** ステージサイズ */
	Int2 mStageSise;

	/** 点と点の距離 = ステージの広さ */
	float mPointDistance;

	/** 最大標高 */
	float mMaxHeight;

	/** 山頂色 */
	ColorData mTopColor;
	/** 地面色 */
	ColorData mGroundColor;
	
	/** プリミティブバッチ描画のためのデータ群 */
	DataForPrimitiveBatchRender<DirectX::VertexPositionColorTexture>*
		mpLineBatchDatas;

	/** ステージの中央地点 */
	DirectX::SimpleMath::Vector3 mCenterPosition;

	/** StageTriangleのマネージャ */
	StageTriangleManager* mpStageTriangleManager;

	/** ステージの端座標（4辺） */
	BoxParam<float> mStageSidePosBox;

	/** 光らせ担当 */
	std::unique_ptr<BrightChanger> mpBright;

	/** ステージポリゴン描画時に使う呼び出し順 */
	std::vector<uint16_t> mIndices;

	/** ステージポリゴン描画時に使う頂点情報 */
	std::vector<DirectX::VertexPositionColorTexture>
		mVertices;
private:

	/**
	* @brief コンストラクタ
	*/
	StageManager();

	/**
	* @brief デストラクタ
	*/
	~StageManager();

public:
	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

	/**
	* @brief 終了処理
	*/
	void Finalize();

public:
	/**
	 * @brief	StageTriangleメンバの追加
	 * @param[in] triangle	追加するﾄﾗｲｱﾝｸﾞﾙ
	 */
	void AddTriangleMember(StageTriangle* triangle);
	/**
	 * @brief	StageTriangleメンバの除外
	 * @param[in] triangle	除外するﾄﾗｲｱﾝｸﾞﾙ
	 */
	void RemoveTriangleMember(StageTriangle* triangle);
	/**
	 * @brief	StageTriangleManagerの取得
	 * @return	StageTriangleManagerのポインタ
	 */
	inline StageTriangleManager* GetStageTriangleManager() {
		return mpStageTriangleManager; 
	}

private:

	/**
	 * @brief	StageTriangleたちの値設定
	 */
	void CalculateStageTriangle();

	/**
	 * @brief	面描画時に使う値たちの設定
	 */
	void SetPolygonDatas();
public:
	/**
	 * @brief	最大標高の取得
	 */
	inline float GetMaxHeight() { return mMaxHeight; }

	/**
	 * @brief	山頂色の取得
	 */
	inline const ColorData& GetTopColor() { return mTopColor; }

	/**
	 * @brief	地面色の取得
	 */
	inline const ColorData& GetGroundColor() { return mGroundColor; }

	/**
	 * @brief	中央座標の取得
	 */
	inline const DirectX::SimpleMath::Vector3& GetCenterPos() { return mCenterPosition; }

	/**
	 * @brief	ステージの端座標の取得
	 */
	inline const BoxParam<float>& GetSidePosBox() { return mStageSidePosBox; }
	
	/**
	 * @brief	全ステージポイントの取得
	 */
	inline const std::vector<std::vector<StagePoint*>>& GetAllStagePoints() 
	{ return mpStagePoints; }

	/**
	 * @brief	ステージサイズ（要素数）の取得
	 */
	inline Int2 GetStageSize() { return mStageSise; }
};