/**
 * @file IScene.h
 * @brief �e�V�[���̌��ƂȂ钊�ۃN���X
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#pragma once

 /**
 * @brief �e�V�[���̌��ƂȂ钊�ۃN���X
 */
class IScene {
public:

	/**
	* @brief �R���X�g���N�^
	*/
	IScene() = default;

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~IScene() = default;

	/**
	* @brief ������
	*/
	virtual void Initialize() = 0;

	/**
	* @brief �X�V
	*/
	virtual void Update() = 0;

	/**
	* @brief �`��
	*/
	virtual void Render() = 0;

	/**
	* @brief �I������
	*/
	virtual void Finalize() = 0;

	/**
	 * @brief �t�F�[�h�J�n�ʒm
	 */
	virtual void OnFadeStart() = 0;

	/**
	 * @brief �t�F�[�h�I���ʒm
	 */
	virtual void OnFadeEnd() = 0;

	/**
	 * @brief �t�F�[�h�X�V�֐�
	 * @return �t�F�[�h���I�������true
	 */
	virtual bool FadeUpdate() = 0;

	/**
	 * @fn
	 * @brief �Ȃ�
	 * @param[in] srv �}�X�N�p�V�F�[�_���\�[�X�r���[
	 */
	virtual void FadeRender(ID3D11ShaderResourceView* srv) = 0;
};