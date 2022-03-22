/**
 * @file BrightChanger.cpp
 * @brief ���̃N���X�̕`�揈�������点��p�N���X
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
//		�R���X�g���N�^
//=====================================================
BrightChanger::BrightChanger(bool isCross)
	:ScreenEffectBase()
{
	// �V�F�[�_���̏�����
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
//		�R���X�g���N�^
//=====================================================
CrossBright::CrossBright()
	:ScreenEffectBase()
{
	// �V�F�[�_���̏�����
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

	// �����_�[�^�[�Q�b�g�����ɖ߂�
	/*pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		&mDefaultRTV,
		depthStencil);*/


	for (auto& bright : mpBrights) {
		bright->Begin();

		// �����`��
		this->Render();

		bright->End(true);
	}


	FLOAT color[] = { 0,0,0,0 };

	// ���O�����_�[�^�[�Q�b�g�r���[���N���A
	context->ClearRenderTargetView(
		mpScreenEffectDatas->mRenderTargetView.Get(),
		color
	);
}
