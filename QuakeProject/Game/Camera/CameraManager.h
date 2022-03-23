/**
 * @file CameraManager.h
 * @brief �Q�[�����ɑ��݂���S�ẴJ�������Ǘ�����
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Component/ComponentManager.h"
#include "MyCamera.h"
#include "Game/Camera/DebugCamera.h"

class ControlCamera;
class Camera;

/**
* @brief �Q�[�����ɑ��݂���S�ẴJ�������Ǘ�����
*/
class CameraManager 
	: public SingletonFounder<CameraManager>
	, public ComponentManager<MyCamera>
{
	friend SingletonFounder;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	CameraManager();

	/**
	* @brief �f�X�g���N�^
	*/
	~CameraManager() = default;
private:
	/** ���ݎB�e��S�����Ă���J���� */
	Camera* mpActiveCamera;

	/** �f�o�b�O�J���� */
	std::unique_ptr<DebugCamera> mpDebugCamera;

	/** �R���g���[���J���� */
	ControlCamera* mpControlCamera;

public:


	/**
	* @brief �X�V
	*/
	void Update() override;

	/**
	* @brief �`��
	*/
	void Render() override;

public:
	/**
	 * @brief	ControlCamera�𐶐��A�擾
	 * @param[in] pOwner	�J���������I�[�i�[�̃|�C���^
	 * @return SplitDataType �J�����̃|�C���^
	 */
	ControlCamera* SpawnControlCamera(Actor* pOwner);

	/**
	 * @brief	�A�N�e�B�u�J������ύX����
	 * @param[in] id	�ύX����J�����̃R���|�[�l���gID
	 */
	inline void ChangeActiveCamera(int id) {
		mpActiveCamera = this->GetMemberFromID(id);
	}

public:
	/**
	 * @brief	�g�p���̃J������view�s���Ԃ�
	 * @return view �r���[�s��
	 */
	const DirectX::SimpleMath::Matrix& GetViewMatrix();

	/**
	 * @brief	�g�p���̃J������projection�s���Ԃ�
	 * @return projection �v���W�F�N�V�����s��
	 */
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix();

};