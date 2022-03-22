/**
 * @file AfterGlow.h
 * @brief �`����ʂ������N���X�̊��N���X
 * @author suzuki.shunsuke
 * @date 2022/03/14
 */
#pragma once
#include "ScreenEffectDatas.h"
#include "Game/Shader/ShaderDatas.h"

 /**
  * @brief �`����ʂ������N���X�̊��N���X
  */
class ScreenEffectBase {
protected:
	/** ��ʌ��ʗp�f�[�^�Q */
	std::unique_ptr<ScreenEffectDatas> mpScreenEffectDatas;

	/** �����_�[�^�[�Q�b�g�r���[(�f�t�H���g�l�ꎞ�ۑ��p) */
	ID3D11RenderTargetView* mDefaultRTV;

	/** �V�F�[�_�p�f�[�^�Q */
	ShaderDataPack mShaderDataPack;
public:
	
	/**
	* @brief �R���X�g���N�^
	*/
	ScreenEffectBase();

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~ScreenEffectBase() = default;

	/**
	* @brief �萔�o�b�t�@�l�ύX
	*/
	virtual void ChangeConstBuffer(const DirectX::SimpleMath::Vector4& cBufParam) = 0;

	/**
	* @brief �`����ʊJ�n
	*/
	virtual void Begin();

	/**
	* @brief �`����ʏI��
	*/
	virtual void End(bool isBlendAdd = false);
	/**
	* @brief ���ʖ{�`��
	*/
	void Render();

protected:

	/**
	* @brief �萔�o�b�t�@�l�ݒ�
	*/
	void SetConstBuf(const DirectX::SimpleMath::Vector4& cBufParam) {
		mShaderDataPack.mTimeParam = cBufParam;
	}
};