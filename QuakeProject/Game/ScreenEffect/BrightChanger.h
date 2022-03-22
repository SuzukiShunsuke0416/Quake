/**
 * @file BrightChanger.h
 * @brief ���̃N���X�̕`�揈�������点��p�N���X
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#pragma once
#include "ScreenEffectBase.h"
#include <vector>

 /**
 * @brief ���̃N���X�̕`�揈�������点��p�N���X
 */
class BrightChanger:public ScreenEffectBase{
private:

public:

	/**
	* @brief �R���X�g���N�^
	*/
	BrightChanger(bool isCross = false);

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~BrightChanger() = default;

	/**
	* @brief �萔�l�ύX
	* @param[in] cBufParam.x ���̕`����n�b�L���c�����S������
	*						�i�c��=0.0f�@�S����=1.0f�j
	* @param[in] cBufParam.y ���Z�̋��� (��{�l��0.15f)
	* @param[in] cBufParam.z ���̑傫�� (��{�l��0.0013f)
	* @param[in] cBufParam.w �Ȃ�(��ʔ�Ŏg�p����B�ݒ�ς݁B)
	*/
	void ChangeConstBuffer(const DirectX::SimpleMath::Vector4& cBufParam)override {
		ScreenEffectBase::SetConstBuf(cBufParam);
	}
};


class CrossBright :public ScreenEffectBase {
private:

	/** ���点�N���X */
	std::vector<std::unique_ptr<BrightChanger>> mpBrights;
public:

	/**
	* @brief �R���X�g���N�^
	*/
	CrossBright();

	/**
	* @brief �f�X�g���N�^
	*/
	~CrossBright() = default;

	/**
	* @brief �萔�l�ύX
	* @param[in] cBufParam.x ���̕`����n�b�L���c�����S������
	*						�i�c��=0.0f�@�S����=1.0f�j
	* @param[in] cBufParam.y ���Z�̋��� (��{�l��0.15f)
	* @param[in] cBufParam.z ���̑傫�� (��{�l��0.0013f)
	* @param[in] cBufParam.w �Ȃ�(��ʔ�Ŏg�p����B�ݒ�ς݁B)
	*/
	void ChangeConstBuffer(const DirectX::SimpleMath::Vector4 & cBufParam)override {
		ScreenEffectBase::SetConstBuf(cBufParam);
	}

	/**
	* @brief �`����ʊJ�n
	*/
	//void Begin() override;

	/**
	* @brief �`����ʏI��
	*/
	void End(bool isBlendAdd = false) override;
};