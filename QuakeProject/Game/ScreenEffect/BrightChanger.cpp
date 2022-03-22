/**
 * @file BrightChanger.cpp
 * @brief 他のクラスの描画処理を光らせる用クラス
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#include "pch.h"
#include "BrightChanger.h"
#include "DeviceResources.h"
#include "Libraries/MyLibraries/Utility.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
BrightChanger::BrightChanger(bool isCross)
	:ScreenEffectBase()
{
	// シェーダ情報の初期化
	if (isCross) {
		mShaderDataPack.Initialize(
			L"Resources/Shaders/ParticleVS.cso",
			L"Resources/Shaders/BrightLitePS.cso"
		);
	}
	else {
		mShaderDataPack.Initialize(
			L"Resources/Shaders/ParticleVS.cso",
			L"Resources/Shaders/BrightPS.cso"
		);
	}
	
}

//-------------------------------------------------------------

//=====================================================
//		コンストラクタ
//=====================================================
CrossBright::CrossBright()
	:ScreenEffectBase()
{
	// シェーダ情報の初期化
	mShaderDataPack.Initialize(
		L"Resources/Shaders/ParticleVS.cso",
		L"Resources/Shaders/LuminancePS.cso"
	);

	for (int i = 1; i < 15; i++) {
		mpBrights.push_back(std::make_unique<BrightChanger>(true));
		mpBrights.back()->ChangeConstBuffer({ 0.0f, 1.5f / i, 0.003f * i * i *0.02f, 0.0f });
	}
}

void CrossBright::End(bool isBlendAdd)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	auto depthStencil = pDR->GetDepthStencilView();

	// レンダーターゲットを元に戻す
	/*pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		&mDefaultRTV,
		depthStencil);*/


	for (auto& bright : mpBrights) {
		bright->Begin();

		// 発光描画
		this->Render();

		bright->End(true);
	}


	FLOAT color[] = { 0,0,0,0 };

	// 自前レンダーターゲットビューをクリア
	context->ClearRenderTargetView(
		mpScreenEffectDatas->mRenderTargetView.Get(),
		color
	);
}
