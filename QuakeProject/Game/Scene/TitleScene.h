/**
 * @file TitleScene.h
 * @brief �^�C�g���V�[��
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#pragma once
#include "IScene.h"
#include "SceneChangeFader.h"

 /**
 * @brief �^�C�g���V�[��
 */
class TitleScene :public IScene {
private:
	/** �V�[���؂�ւ��S�� */
	SceneChangeFader mSceneChangeFader;
public:
	/**
	* @brief �R���X�g���N�^
	*/
	TitleScene();

	/**
	* @brief �f�X�g���N�^
	*/
	~TitleScene();

	/**
	 * @brief �������֐�
	 */
	void Initialize() override;

	/**
	 * @brief �X�V�֐�
	 */
	void Update() override;

	/**
	 * @brief �`��֐�
	 */
	void Render() override;

	/**
	 * @brief �I���֐�
	 */
	void Finalize() override;

	/**
	 * @brief �t�F�[�h�J�n�ʒm
	 */
	void OnFadeStart() override;

	/**
	 * @brief �t�F�[�h�I���ʒm
	 */
	void OnFadeEnd() override;

	/**
	 * @brief �t�F�[�h�X�V�֐�
	 * @return	�t�F�[�h���I�������true
	 */
	bool FadeUpdate() override;

	/**
	 * @fn
	 * @brief �Ȃ�
	 * @param[in] srv �}�X�N�p�V�F�[�_���\�[�X�r���[
	 */
	void FadeRender(ID3D11ShaderResourceView* srv) override;
};