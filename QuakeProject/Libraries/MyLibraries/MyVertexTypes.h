/**
 * @file MyVertexType.h
 * @brief �v���~�e�B�u�o�b�`�Ȃǂ�`�悷��Ƃ��Ɏg���f�[�^�Q
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

	 �v���~�e�B�u�o�b�`&���_�^���܂Ƃ߂��^�̕ʖ���`����

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
	
	�v���~�e�B�u�o�b�`��`�悷��Ƃ��ɕK�v�ȃf�[�^�Q���֗��Ȋ֐�

=========================================================================================*/
template <class T>
struct DataForPrimitiveBatchRender {
public:
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] isTextureFlag�@�e�N�X�`����K�������邩�B
		(false�̏ꍇ�͐F��K�����鎖�ɂȂ�)
	 */
	DataForPrimitiveBatchRender(bool isTextureFlag = false);

	//! ���X�^���C�U�X�e�[�g�ݒ�
	void SetRSState(ID3D11RasterizerState* rsstate) {
		pRSstate = rsstate;
	}

	//! �x�[�V�b�N�G�t�F�N�g�K��
	void BasicEffectApply(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity
	);

public:

	//! ���_�^
	using VertexType = T;
	
	// �G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> basicEffect;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<T>> primitiveBatch;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> commonState;

private:

	// ���X�^���C�U�X�e�[�g�i�J�����O�n�ݒ�j
	ID3D11RasterizerState* pRSstate;
};

/*=========================================================================================

	�v���~�e�B�u�o�b�`��`�悷��Ƃ��ɕ֗��Ȋ֐�

=========================================================================================*/

//! �R���X�g���N�^
template<class T>
inline DataForPrimitiveBatchRender<T>::DataForPrimitiveBatchRender(bool isTextureFlag)
	:basicEffect()
	,primitiveBatch()
	,inputLayout()
	,commonState()
	,pRSstate()
{
	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// �G�t�F�N�g�̐���
	basicEffect = std::make_unique<DirectX::BasicEffect>(device);

	// ���_�J���[�̐ݒ�
	basicEffect->SetLightingEnabled(false);   //���C�goff
	basicEffect->SetTextureEnabled(isTextureFlag);	   //�e�N�X�`��
	basicEffect->SetVertexColorEnabled(!isTextureFlag);//���_�J���[

	// �v���~�e�B�u�o�b�`�̐���
	primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<T>>(context);

	// �C���v�b�g���C�A�E�g�̐ݒ�
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

	//! �R�����X�e�[�g�̐���
	commonState = std::make_unique<DirectX::CommonStates>(device);

	// �J�����O�����ݒ�
	pRSstate = commonState->CullCounterClockwise();
}

//! �x�[�V�b�N�G�t�F�N�g�K��
template<class T>
inline void DataForPrimitiveBatchRender<T>::BasicEffectApply(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	DirectX::SimpleMath::Matrix world)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	//! �C���v�b�g���C�A�E�g�ݒ�
	context->IASetInputLayout(inputLayout.Get());
	//! �u�����h�X�e�[�g
	context->OMSetBlendState(commonState->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	//! �[�x�o�b�t�@�ݒ�
	context->OMSetDepthStencilState(commonState->DepthDefault(), 0);
	//! �J�����O�͔����v���
	context->RSSetState(pRSstate);

	basicEffect->SetWorld(world);
	basicEffect->SetView(view);
	basicEffect->SetProjection(proj);
	basicEffect->Apply(context);
}

