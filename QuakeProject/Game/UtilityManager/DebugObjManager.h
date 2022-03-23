/**
 * @file DebugObjManager.h
 * @brief 3Dオブジェクトを描画する専用のマネージャー
 * @author suzuki.shunsuke
 * @date 2022/02/27
 */
#pragma once
#include "SingletonFounder.h"
#include <vector>
#include <GeometricPrimitive.h>
#include <SimpleMath.h>
#include "Libraries/MyLibraries/MyVertexTypes.h"

/**
 * @brief 3Dオブジェクトを描画する専用のマネージャー
 */
class DebugObjManager :public SingletonFounder<DebugObjManager> {
public:
	friend SingletonFounder;

private:
	/**
	 * @brief スフィア用のデータ群
	 */
	struct DebugObjData_Sphere {
		DirectX::SimpleMath::Vector3 pos;
		float scale;
		DirectX::SimpleMath::Vector3 color;
	};

	/**
	 * @brief キューブ用のデータ群
	 */
	struct DebugObjData_Cube {
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Vector3 color;
	};

	/**
	 * @brief  ティーポット用のデータ群（別名定義）
	 */
	using DebugObjData_TeaPot = DebugObjData_Cube;

	/**
	 * @brief ライン用のデータ群
	 */
	struct DebugObjData_Line {
		DirectX::SimpleMath::Vector3 StartPos;
		DirectX::SimpleMath::Vector3 EndPos;
		DirectX::SimpleMath::Vector4 StartColor;
		DirectX::SimpleMath::Vector4 EndColor;
	};


	/** スフィア描画データ用配列 */
	std::vector<DebugObjData_Sphere> mSphereDataVec;
	/** キューブ描画データ用配列 */
	std::vector<DebugObjData_Cube> mCubeDataVec;
	/** ティーポット描画データ用配列 */
	std::vector<DebugObjData_TeaPot> mTeaPotDataVec;
	/** ライン描画データ用配列 */
	std::vector<DebugObjData_Line> mLineDataVec;

	/** ジオメトリックプリミティブ（スフィア用） */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPri;
	/** ジオメトリックプリミティブ（キューブ用） */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPriCube;
	/** ジオメトリックプリミティブ（ティーポット用） */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPriTeaPot;
	/** プリミティブバッチ（ライン用） */
	std::unique_ptr<DataForPrimitiveBatchRender<DirectX::VertexPositionColor>>
		mpPriLine;

private:
	/**
	* @brief コンストラクタ
	*/
	DebugObjManager();

public:
	/**
	* @brief デストラクタ
	*/
	~DebugObjManager() {

	}

	/**
	* @brief 描画
	 * @param[in] view	ビュー行列
	 * @param[in] proj	プロジェクション行列
	*/
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj);

	/**
	 * @brief	描画申請（スフィア）
	 * @param[in]	pos		座標
	 * @param[in]	scale	スケール
	 * @param[in]	color	色
	 */
	inline void Entry(const DirectX::SimpleMath::Vector3& pos,
		float scale = 1.0f,
		DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Vector3::One)
	{
		mSphereDataVec.push_back({pos,scale,color });
	}

	/**
	 * @brief	描画申請（キューブ）
	 * @param[in]	world	モデル行列
	 * @param[in]	color	色
	 */
	inline void EntryCube(const DirectX::SimpleMath::Matrix& world,
		DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Vector3::One)
	{
		mCubeDataVec.push_back({ world,color });
	}

	/**
	 * @brief	描画申請（ティーポット）
	 * @param[in]	world	モデル行列
	 * @param[in]	color	色
	 */
	inline void EntryTeaPot(const DirectX::SimpleMath::Matrix& world,
		DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Vector3::One)
	{
		mTeaPotDataVec.push_back({ world,color });
	}

	/**
	 * @brief	描画申請（ライン）
	 * @param[in]	startPos	始点
	 * @param[in]	endPos		終点
	 * @param[in]	startColor	始点色
	 * @param[in]	endColor	終点色
	 */
	inline void EntryLine(
		const DirectX::SimpleMath::Vector3& startPos,
		const DirectX::SimpleMath::Vector3& endPos,
		const DirectX::SimpleMath::Vector4& startColor = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		const DirectX::SimpleMath::Vector4& endColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f,1.0f)) 
	{
		mLineDataVec.push_back({ startPos,endPos,startColor,endColor });
	}

private:

	/**
	* @brief 配列を空にする
	*/
	inline void Clear() {
		mSphereDataVec.clear();
		mCubeDataVec.clear();
		mTeaPotDataVec.clear();
		mLineDataVec.clear();
	}
};