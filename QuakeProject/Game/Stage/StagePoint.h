/**
 * @file StagePoint.h
 * @brief ステージを構成する１つの点（アクター）
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/Actor/Actor.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Game/ElevationColor/ElevationColor.h"
#include "Libraries/MyLibraries/MyVertexTypes.h"
#include <unordered_set>
#include "Libraries/MyLibraries/PerlinNoise.h"

class StagePoint;

/**
* @brief StagePointの初期化に必要な情報群
*/
struct StagePointInitDesc {
	DirectX::SimpleMath::Vector3 pos;
	Int2 element;
	StagePoint* nextRight;
	StagePoint* nextBottom;
	StagePoint* nextRightBottom;
	DataForPrimitiveBatchRender<DirectX::VertexPositionColorTexture>* 
		pLineBatchDatas;
};

/**
* @brief ステージを構成する１つの点
*/
class StagePoint :public Actor{
public:

private:
	/** 自分の番地(配列の要素)番号 */
	Int2 mElement;

	/** 隣接点のptr（右） */
	StagePoint* mpNextRight;
	/** 隣接点のptr（下） */
	StagePoint* mpNextBottom;
	/** 隣接点のptr（右下） */
	StagePoint* mpNextRightBottom;

	/** 隣接点のptrをまとめたもの*/
	std::unordered_set<StagePoint*> mpNextPoints;

	/** 色情報*/
	ElevationColor* mpColor;

	/** プリミティブバッチデータ群*/
	DataForPrimitiveBatchRender<DirectX::VertexPositionColorTexture>* 
		mpPrimitiveBatchDatas;
public:

	/**
	* @brief コンストラクタ
	*/
	StagePoint();
	/**
	* @brief デストラクタ
	*/
	~StagePoint();

	/**
	 * @brief	初期化
	 * @param[in] desc	初期化データ群
	 * @param[in] noiseMaker ノイズ生成機
	 */
	void Initialize(const StagePointInitDesc& desc, PerlinNoise& noiseMaker);

	/**
	* @brief 更新
	*/
	void Update() override;

	/**
	* @brief 面の描画
	*/
	void Render() override;

	/**
	* @brief 線の描画
	*/
	void LineRender();

	/**
	* @brief 高さ（座標）を取得
	*/
	float GetLocationY() {
		return mTransform.Location.y;
	}

	/**
	* @brief 色情報の取得
	*/
	ElevationColor* GetElevationColor() {
		return mpColor;
	}
};