/**
 * @file SceneChangeFader.h
 * @brief �V�[���؂�ւ����̃t�F�[�h��S������N���X
 * @author suzuki.shunsuke
 * @date 2022/01/30
 */
#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <unordered_map>
#include <functional>

 /**
 * @brief �t�F�[�h�̎��
 */
enum class eSceneChangeFadeType :unsigned int{
	Normal,

	TYPE_NUM
};

/**
 * @brief �V�[���؂�ւ����̃t�F�[�h��S������N���X
 */
class SceneChangeFader {
private:
	/** �ʍX�V�֐��̌^ */
	using updateMold = std::function<bool(SceneChangeFader*)>;
	/** �X�V�������܂Ƃ߂��z��i�ÓI�j */
	static const updateMold 
		msUpdateFunctions[(unsigned int)(eSceneChangeFadeType::TYPE_NUM)];

	/** �ʏ�p�����[�^�̏I���l */
	static const float NORMAL_PARAM_END;

private:
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

	/** �e�I�u�W�F�N�g���S������X�V�֐� */
	updateMold mUpdate;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	SceneChangeFader();
	/**
	* @brief �f�X�g���N�^
	*/
	~SceneChangeFader() = default;

	/**
	 * @brief �������֐�
	 * @param[in] type �t�F�[�h�^�C�v
	 */
	void Initialize(eSceneChangeFadeType type);

	/**
	 * @brief �X�V
	 * @return �t�F�[�h���I��������ǂ���
	 */
	bool Update();

	/**
	 * @brief �`��
	 * @param[in] srv �}�X�N�p�V�F�[�_���\�[�X�r���[
	 */
	void Draw(ID3D11ShaderResourceView* srv);

private:
	/**
	 * @brief �ʏ�X�V�֐��@�i�P�̃p�����[�^�������ő�����̂݁j
	 * @return �l���I���l�ɂȂ�����
	 */
	bool NormalUpdate();
};