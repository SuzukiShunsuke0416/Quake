//
// GridFloor.cpp
//
#include "pch.h"
#include "GridFloor.h"
#include "DeviceResources.h"

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
GridFloor::GridFloor(ID3D11Device1* device, ID3D11DeviceContext1* context, const int divsX, const int divsY)
	: mDivsX(divsX)
	, mDivsY(divsY)	
{
	// エフェクトの生成
	mBasicEffect = std::make_unique<DirectX::BasicEffect>(device);

	// 頂点カラーの設定
	mBasicEffect->SetVertexColorEnabled(true);
	
	// プリミティブバッチの生成
	mPrimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
	
	// インプットレイアウトの設定
	void const* shaderByteCode;
	size_t byteCodeLength;
	mBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, 
		byteCodeLength,
		mInputLayout.GetAddressOf()
	);
}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
GridFloor::~GridFloor()
{
}

//-------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------
void GridFloor::Draw(const GDraw_Desc& gdd)
{
	EApply_Desc ead;
	ead.states = gdd.states;
	ead.view = gdd.view;
	ead.proj = gdd.proj;

	this->EffectApply(ead);

	mPrimitiveBatch->Begin();

	const DirectX::XMVECTORF32 axisX = { (float)mDivsX, 0.0f, 0.0f };
	const DirectX::XMVECTORF32 axisY = { 0.0f, 0.0f, (float)mDivsY };
	const DirectX::SimpleMath::Vector4 color = DirectX::Colors::Gray;

	size_t divsX = std::max<size_t>(1, mDivsX);
	DirectX::FXMVECTOR origin = DirectX::g_XMZero;
	// 垂直線
	for (size_t i = 0; i <= divsX; ++i)
	{
		float scaleFactor = float(i) / float(divsX);
		scaleFactor = (scaleFactor * 1.0f) - 0.5f;
		DirectX::XMVECTOR scaleV = XMVectorScale(axisX, scaleFactor);
		scaleV = DirectX::XMVectorAdd(scaleV, origin);

		DirectX::VertexPositionColor v1(DirectX::XMVectorSubtract(scaleV, axisY * 0.5f), color);
		DirectX::VertexPositionColor v2(DirectX::XMVectorAdd(scaleV, axisY * 0.5f), color);
		mPrimitiveBatch->DrawLine(v1, v2);
	}
	// 水平線
	size_t divsY = std::max<size_t>(1, mDivsY);
	for (size_t i = 0; i <= divsY; i++)
	{
		float scaleFactor = float(i) / float(divsY);
		scaleFactor = (scaleFactor * 1.0f) - 0.5f;
		DirectX::XMVECTOR scaleV = XMVectorScale(axisY, scaleFactor);
		scaleV = DirectX::XMVectorAdd(scaleV, origin);

		DirectX::VertexPositionColor v1(DirectX::XMVectorSubtract(scaleV, axisX * 0.5f), color);
		DirectX::VertexPositionColor v2(DirectX::XMVectorAdd(scaleV, axisX * 0.5f), color);
		mPrimitiveBatch->DrawLine(v1, v2);
	}

	mPrimitiveBatch->End();
}

void GridFloor::EffectApply(const EApply_Desc& ead)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	context->IASetInputLayout(mInputLayout.Get());
	context->OMSetBlendState(ead.states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(ead.states->DepthDefault(), 0);

	mBasicEffect->SetWorld(world);
	mBasicEffect->SetView(ead.view);
	mBasicEffect->SetProjection(ead.proj);
	mBasicEffect->Apply(context);
}
