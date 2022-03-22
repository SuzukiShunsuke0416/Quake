/**
 * @file AfterGlow.h
 * @brief ���̃N���X�̕`�揈���Ɏc����t�^����N���X
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#pragma once
#include <functional>
#include "BrightChanger.h"
#include <Model.h>
#include <deque>

 /**
 * @brief �c����t�^����N���X
 */
class AfterGlow{
private:

	/** �����p�N���X */
	std::unique_ptr<BrightChanger> mpBright;

	/** �c���I�u�W�F�̏k������ */
	float mReductionRat;

	/** �c���I�u�W�F�̕`�搔 */
	int mGlowSize;

	/** ���f���s��̃��O */
	std::deque<DirectX::SimpleMath::Matrix> mModelMatrixLogs;

	/** ���f���s�񃍃O�ύX�֐���ptr */
	std::function<void(AfterGlow*, const DirectX::SimpleMath::Matrix&)>
		mUpdateLogFunction;
public:

	/**
	* @brief �R���X�g���N�^
	* @param[in] reductionRat �c���I�u�W�F�����̏k������
	* @param[in] glowSize �c���I�u�W�F�̐�
	*/
	AfterGlow(float reductionRat = 0.9f, int glowSize = 32);

	/**
	* @brief �f�X�g���N�^
	*/
	~AfterGlow() = default;

	/**
	* @brief �c����K��������`�揈��
	* @param[in] pGeoPri �޵��د�����è�ނ̃|�C���^
	* @param[in] ModelMat ���f���s��
	* @param[in] color �޵��د�����è�ނ̐F
	*/
	void Adaptation(
		DirectX::GeometricPrimitive* pGeoPri,
		DirectX::SimpleMath::Matrix ModelMat,
		DirectX::XMVECTORF32 color
	);

	/**
	* @brief �c����K��������`�揈��
	* @param[in] pModel ���f���̃|�C���^
	* @param[in] pCommonStates �R�����X�e�[�g�̃|�C���^
	* @param[in] ModelMat ���f���s��
	*/
	void Adaptation(
		DirectX::Model* pModel,
		DirectX::CommonStates* pCommonStates,
		DirectX::SimpleMath::Matrix ModelMat
	);

private:
	/**
	* @brief ���f���s��̃��O��ǉ�����
	* @param[in] ModelMat ���f���s��
	*/
	void PushModelMat(const DirectX::SimpleMath::Matrix& modelMat);

	/**
	* @brief ���f���s��̃��O��ǉ����A����Ȃ����͍폜����
	* @param[in] ModelMat ���f���s��
	*/
	void PushPopModelMat(const DirectX::SimpleMath::Matrix& modelMat);
};