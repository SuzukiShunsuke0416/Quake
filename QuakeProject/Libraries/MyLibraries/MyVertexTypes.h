/**
 * @file MyVertexType.h
 * @brief プリミティブバッチなどを描画するときに使うデータ群
 * @author suzuki.shunsuke
 * @date 2022/02/06
 */
#pragma once
#include "pch.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "Libraries/DirectXTK/inc/Effects.h"
#include <CommonStates.h>
#include "DeviceResources.h"

 /*=========================================================================================

	 プリミティブバッチ&頂点型をまとめた型の別名定義たち

 =========================================================================================*/

namespace PrimitiveBatchVertexTypes {
	//! PrimitiveBatch_VertexPosition
	using VerPos		= DirectX::PrimitiveBatch<DirectX::VertexPosition>;
	
	//! PrimitiveBatch_VertexPositionColor
	using VerPosCol		= DirectX::PrimitiveBatch<DirectX::VertexPositionColor>;
	
	//! PrimitiveBatch_VertexPositionTexture
	using VerPosTex		= DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>;
	
	//! PrimitiveBatch_VertexPositionColorTexture
	using VerPosColTex	= DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>;
}

/*=========================================================================================
	
	プリミティブバッチを描画するときに必要なデータ群＆便利な関数

=========================================================================================*/
template <class T>
struct DataForPrimitiveBatchRender {
public:
	/**
	 * @brief	コンストラクタ
	 * @param[in] isTextureFlag　テクスチャを適応させるか。
		(falseの場合は色を適応する事になる)
	 */
	DataForPrimitiveBatchRender(bool isTextureFlag = false);

	//! ラスタライザステート設定
	void SetRSState(ID3D11RasterizerState* rsstate) {
		pRSstate = rsstate;
	}

	//! ベーシックエフェクト適応
	void BasicEffectApply(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity
	);

public:

	//! 頂点型
	using VertexType = T;
	
	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> basicEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<T>> primitiveBatch;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> commonState;

private:

	// ラスタライザステート（カリング系設定）
	ID3D11RasterizerState* pRSstate;
};

/*=========================================================================================

	プリミティブバッチを描画するときに便利な関数

=========================================================================================*/

//! コンストラクタ
template<class T>
inline DataForPrimitiveBatchRender<T>::DataForPrimitiveBatchRender(bool isTextureFlag)
	:basicEffect()
	,primitiveBatch()
	,inputLayout()
	,commonState()
	,pRSstate()
{
	// D3Dデバイスとデバイスコンテキストの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// エフェクトの生成
	basicEffect = std::make_unique<DirectX::BasicEffect>(device);

	// 頂点カラーの設定
	basicEffect->SetLightingEnabled(false);   //ライトoff
	basicEffect->SetTextureEnabled(isTextureFlag);	   //テクスチャ
	basicEffect->SetVertexColorEnabled(!isTextureFlag);//頂点カラー

	// プリミティブバッチの生成
	primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<T>>(context);

	// インプットレイアウトの設定
	void const* shaderByteCode;
	size_t byteCodeLength;
	basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexType::InputElements,
		VertexType::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		inputLayout.GetAddressOf()
	);

	//! コモンステートの生成
	commonState = std::make_unique<DirectX::CommonStates>(device);

	// カリング初期設定
	pRSstate = commonState->CullCounterClockwise();
}

//! ベーシックエフェクト適応
template<class T>
inline void DataForPrimitiveBatchRender<T>::BasicEffectApply(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	DirectX::SimpleMath::Matrix world)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	//! インプットレイアウト設定
	context->IASetInputLayout(inputLayout.Get());
	//! ブレンドステート
	context->OMSetBlendState(commonState->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	//! 深度バッファ設定
	context->OMSetDepthStencilState(commonState->DepthDefault(), 0);
	//! カリングは反時計回り
	context->RSSetState(pRSstate);

	basicEffect->SetWorld(world);
	basicEffect->SetView(view);
	basicEffect->SetProjection(proj);
	basicEffect->Apply(context);
}

