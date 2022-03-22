/**
 * @file SceneManager.h
 * @brief �e�V�[�����Ǘ�����N���X
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */

#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include <vector>
#include <functional>
#include <StepTimer.h>

 /**
  * @enum eSceneType
  * �V�[���̃^�C�v
  */
enum class eSceneType {
	Title,
	Play,
};

class IScene;
class TitleScene;
class PlayScene;

/**
* @brief �e�V�[�����Ǘ�����N���X
*/
class SceneManager :public SingletonFounder<SceneManager> {
	friend SingletonFounder;
private:
	/**
	* @brief �R���X�g���N�^
	*/
	SceneManager();

	/**
	* @brief �f�X�g���N�^
	*/
	~SceneManager();
public:
	/** �����ɏ��������X�V�֐��̐� */
	static const int MAX_FUNCTION_COUNT;

private:
	/** ���O��1f�o�ߎ��� */
	float mElapsedTime;

	/** ���݃��C���œ����Ă���V�[�� */
	IScene* mpActiveScene;
	/** �t�F�[�h���ɕ\�ɏo�Ă���V�[�� */
	IScene* mpStandByScene;

	/** �V�[���}�l�[�W������void()�n�֐�ptr�̌^ */
	using SMfuncType = std::function<void(SceneManager*)> ;

	/** ���ݍX�V����֐� */
	std::vector<SMfuncType> mCurrentUpdates;

	/** ���ݕ`�悷��֐� */
	std::vector<SMfuncType> mCurrentRenders;

	/** 2D�e�N�X�`�� */
	Microsoft::WRL::ComPtr<ID3D11Texture2D> 
		mTexture2D;
	/** �����_�[�^�[�Q�b�g�r���[ */
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> 
		mFadeImageRenderTargetView;
	/** �V�F�[�_�[���\�[�X�r���[ */
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> 
		mFadeImageShaderResourceView;

	/** ���ݓK�p����Ă��郌���_�[�^�[�Q�b�g�r���[ */
	ID3D11RenderTargetView* mpCurrentRTV;
public:

	/**
	 * @brief �������֐�
	 */
	void Initialize();

	/**
	 * @fn
	 * @brief �X�V�֐�
	 * @param[in] StepTimer
	 */
	void Update(DX::StepTimer const& timer);

	/**
	 * @brief �`��֐�
	 */
	void Render();

	/**
	 * @brief �V�[����ύX����
	 * @param[in] nextScene ���̃V�[��
	 */
	void ChangeScene(eSceneType nextScene);

private:
	/**
	 * @brief �A�N�e�B�u�V�[���X�V�֐�
	 */
	void UpdataActiveScene();

	/**
	 * @brief �A�N�e�B�u�V�[���`��֐�
	 */
	void RenderActiveScene();

	/**
	 * @brief �X�^���o�C�V�[���X�V�֐�
	 */
	void UpdateStandByScene();

	/**
	 * @brief �X�^���o�C�V�[���`��֐�
	 */
	void RenderStandByScene();

private:
	/**
	 * @brief ���������֐���ǉ�����(active)
	 */
	void AddFuncActive();

	/**
	 * @brief ���������֐���ǉ�����(stand by)
	 */
	void AddFuncStandBy();

	/**
	 * @brief ���������֐������O����(stand by)
	 */
	void RemoveFuncStandBy();

	/**
	 * @brief �t�F�[�h�pTexture�Ȃǂ�ݒ�
	 */
	void InitFadeTextures();

public:
	/**
	 * @brief ���O��1f�o�ߎ��Ԃ��擾
	 * @return 1f�o�ߎ���
	 */
	inline float GetElapsedTime() { return mElapsedTime; }

	/**
	 * @brief ���ݓK�p����Ă��郌���_�[�^�[�Q�b�g�r���[���擾
	 */
	inline ID3D11RenderTargetView* GetCurrentRTV() {
		return mpCurrentRTV;
	}
};