/**
 * @file PlayScene.h
 * @brief �v���C�V�[��
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#pragma once
#include "IScene.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "SceneChangeFader.h"
#include "Game/Actor/DebugActor.h"

class StageManager;
class CameraManager;
class ElevationColorManager;
class CharacterManager;
class CollisionManager;
class AutoDriveManager;

/**
* @brief �v���C�V�[��
*/
class PlayScene :public IScene {
private:
	/** �V�[���؂�ւ��S�� */
	SceneChangeFader mSceneChangeFader;

	/** �X�e�[�W */
	StageManager* mpStageManager;

	/** �J�����}�l�[�W�� */
	CameraManager* mpCameraManager;

	/** �F�}�l�[�W�� */
	ElevationColorManager* mpColorManager;

	/** �L�����N�^�[�}�l�[�W�� */
	CharacterManager* mpCharacterManager;

	/** �����蔻��}�l�[�W�� */
	CollisionManager* mpCollisionManager;

	// �f�o�b�O�p
	DebugActor mDebugActor;

	/** �������s�}�l�[�W�� */
	AutoDriveManager* mpAutoDriveManager;
public:
	/**
	 * �R���X�g���N�^
	 */
	PlayScene();

	/**
	 * �f�X�g���N�^
	 */
	~PlayScene();

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