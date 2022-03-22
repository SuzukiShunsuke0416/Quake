#pragma once
#include <PrimitiveBatch.h>
/**
 * @file ShaderDatas.h
 * @brief �V�F�[�_���g�����ɕK�v�ȃf�[�^�Q
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
struct ShaderDataPack {
	/** �V�F�[�_�ɓn�����R�g�̒l */
	DirectX::SimpleMath::Vector4 mTimeParam;

	/** �v���~�e�B�u�o�b�` */
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>
		mPrimitiveBatch;

	/** ���_�V�F�[�_ */
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;

	/** �s�N�Z���V�F�[�_ */
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;

	/** �萔�o�b�t�@ */
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCBuffer;

	/** �R�����X�e�[�g */
	std::unique_ptr<DirectX::CommonStates> mStates;

	/** ���̓��C�A�E�g */
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	/**
	* @brief �R���X�g���N�^
	*/
	ShaderDataPack()
		: mTimeParam()
		, mPrimitiveBatch()
		, mVertexShader()
		, mPixelShader()
		, mCBuffer()
		, mStates()
		, mInputLayout() 
	{
	}

	/**
	* @brief ������
	* @param[in] ���_�V�F�[�_�̃p�X
	* @param[in] �s�N�Z���V�F�[�_�̃p�X
	*/
	void Initialize(const wchar_t* VSpath,const wchar_t* PSpath);
};