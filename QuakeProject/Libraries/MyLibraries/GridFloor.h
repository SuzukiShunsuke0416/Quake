//
// GridFloor.h
//
#pragma once

#include <CommonStates.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <SimpleMath.h>
#include <Effects.h>

/// <summary>
/// デバッグ表示用のグリッド床を表示するクラス
/// </summary>

struct EApply_Desc {
	DirectX::CommonStates* states;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix proj;

	EApply_Desc()
		:states(nullptr)
		, view(DirectX::SimpleMath::Matrix::Identity)
		, proj(DirectX::SimpleMath::Matrix::Identity)
	{
	}
};

struct GDraw_Desc {
	DirectX::CommonStates* states;
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix proj;

	GDraw_Desc()
		:states(nullptr)
		, view(DirectX::SimpleMath::Matrix::Identity)
		, proj(DirectX::SimpleMath::Matrix::Identity)
	{
	}
};

class GridFloor
{
	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> mBasicEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> mPrimitiveBatch;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	// 床の一辺のサイズ
	float mSizeX, mSizeY;

	// 分割数
	int mDivsX, mDivsY;

public:
	// コンストラクタ
	GridFloor(
		ID3D11Device1* device, 
		ID3D11DeviceContext1* context, 
		const int divsX,	// 分割数X
		const int divsY		// 分割数Y
	);

	// デストラクタ
	~GridFloor();

	// 描画処理
	void Draw(const GDraw_Desc& gdd);

	//エフェクトの適用部分をラップ
	void EffectApply(const EApply_Desc& ead);
};
