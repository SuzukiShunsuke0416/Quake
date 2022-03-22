/**
 * @file ScreenEffectDatas.h
 * @brief ��ʌ��ʏ������s���Ƃ��ɕK�v�ȃf�[�^�Q
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#pragma once

/**
 * @brief ��ʌ��ʏ������s���Ƃ��ɕK�v�ȃf�[�^�Q
 * @note �����Ɠ����ɕϐ������ɒl�������Z�b�g�����B
 */
struct ScreenEffectDatas {
	/** 2D�e�N�X�`�� */
	Microsoft::WRL::ComPtr<ID3D11Texture2D>
		mTexture2D;
	/** �����_�[�^�[�Q�b�g�r���[ */
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>
		mRenderTargetView;
	/** �V�F�[�_�[���\�[�X�r���[ */
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		mShaderResourceView;

	/**
	* @brief �R���X�g���N�^
	*/
	ScreenEffectDatas();


	/**
	* @brief �f�X�g���N�^
	*/
	~ScreenEffectDatas() = default;
};