#include "pch.h"
#include "ScreenEffectBase.h"
#include "DeviceResources.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Game/Scene/SceneManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
ScreenEffectBase::ScreenEffectBase()
	:mpScreenEffectDatas()
	, mDefaultRTV()
	, mShaderDataPack()
{
	// ��ʌ��ʗp�f�[�^�Q�̐���
	mpScreenEffectDatas = std::make_unique<ScreenEffectDatas>();
}

//=====================================================
//		�`����ʊJ�n
//=====================================================
void ScreenEffectBase::Begin()
{
	// �f�t�H���g�̕`��ݒ�B���Ăяo��
	auto pDR = DX::DeviceResources::GetInstance();
	//mDefaultRTV = pDR->GetRenderTargetView();
	mDefaultRTV = SceneManager::GetInstance()->GetCurrentRTV();
	auto depthStencil = pDR->GetDepthStencilView();

	// �����_�����O������̃N���X���p�ӂ����ޔ�p�ɐ؂�ւ���
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		mpScreenEffectDatas->mRenderTargetView.GetAddressOf(),
		depthStencil
	);

}

//=====================================================
//		�`����ʏI��
//=====================================================
void ScreenEffectBase::End(bool isBlendAdd)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	auto depthStencil = pDR->GetDepthStencilView();

	// �����_�[�^�[�Q�b�g�����ɖ߂�
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		&mDefaultRTV,
		depthStencil);

	// ���Z�����H
	if (isBlendAdd) {
		ID3D11BlendState* blendstate = mShaderDataPack.mStates->Additive();
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	}
	else {
		ID3D11BlendState* blendstate = mShaderDataPack.mStates->NonPremultiplied();
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	}
	

	// �����`��
	this->Render();

	FLOAT color[] = { 0,0,0,0 };

	// ���O�����_�[�^�[�Q�b�g�r���[���N���A
	context->ClearRenderTargetView(
		mpScreenEffectDatas->mRenderTargetView.Get(),
		color
	);

}

//=====================================================
//		���ʖ{�`�拤�ʏ���
//=====================================================
void ScreenEffectBase::Render()
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	// ��ʔ���v�Z
	RECT screenSize = pDR->GetOutputSize();
	mShaderDataPack.mTimeParam.w 
		= (float)(screenSize.bottom) / (float)(screenSize.right);

	//�V�F�[�_�ɓn���p�����[�^�̐ݒ�
	ConstBuffer cbuff;
	cbuff.matView = Matrix::Identity.Transpose();
	cbuff.matProj = Matrix::Identity.Transpose();
	cbuff.matWorld = Matrix::Identity.Transpose();
	cbuff.Time = mShaderDataPack.mTimeParam;

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(mShaderDataPack.mCBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(mShaderDataPack.mStates->DepthRead(), 0);
	// �J�����O
	context->RSSetState(mShaderDataPack.mStates->CullNone());

	//�萔�o�b�t�@���V�F�[�_�ɓn��
	ID3D11Buffer* cb[1] = { mShaderDataPack.mCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);


	//�T���v���[�A�V�F�[�_�A�摜�����ꂼ��o�^
	ID3D11SamplerState* sampler[1] = { mShaderDataPack.mStates->LinearClamp() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(mShaderDataPack.mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mShaderDataPack.mPixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, mpScreenEffectDatas->mShaderResourceView.GetAddressOf());

	//���̓��C�A�E�g�𔽉f
	context->IASetInputLayout(mShaderDataPack.mInputLayout.Get());

	// ���O����������̂ňꎞ�I�ɏȗ�
	using VertexPCT = VertexPositionColorTexture;

	// �S���
	VertexPCT ver[] = {
		VertexPCT(Vector3(-1.0f,1.0f,0.0f),Vector4::Zero,Vector2(0.0f,0.0f)),
		VertexPCT(Vector3(1.0f,1.0f,0.0f),Vector4::Zero,Vector2(1.0f,0.0f)),
		VertexPCT(Vector3(1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(1.0f,1.0f)),
		VertexPCT(Vector3(-1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(0.0f,1.0f))
	};

	//�o�b�`�ɒ��_����n��
	mShaderDataPack.mPrimitiveBatch->Begin();
	mShaderDataPack.mPrimitiveBatch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	mShaderDataPack.mPrimitiveBatch->End();


	//���̃��f���ɉe�����o��\��������̂Ŏg���I�������V�F�[�_���O��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
