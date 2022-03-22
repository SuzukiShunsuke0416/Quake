/**
 * @file Actor.h
 * @brief �X�e�[�W�ɔz�u�����I�u�W�F�N�g�̊��N���X
 * @author suzuki.shunsuke
 * @date 2022/02/01
 */
#pragma once
#include "Libraries/MyLibraries/Transform.h"
#include <unordered_map>
#include <vector>

class Component;

/**
* @brief �X�e�[�W�ɔz�u�����I�u�W�F�N�g�̊��N���X
*/
class Actor {
protected://�S�A�N�^�[���K�����v�f

	/** ���W�E��]�E�g�k�p�����[�^ */
	Transform mTransform;

	/** �A�N�^�[ID */
	int mActorID;

private://�A�N�^�[�ɂ���ē��e���ς��v�f

	/** �R���|�[�l���g�z�� */
	std::unordered_map<int,Component*> mpComponents;
	/** �R���|�[�l���gID�z�� */
	std::vector<int> mpComponentIDs;
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Actor();

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~Actor();


	/**
	* @brief �X�V
	*/
	virtual void Update() = 0;

	/**
	* @brief �`��
	*/
	virtual void Render() = 0;

public:
	/**
	 * @brief �R���|�[�l���g�̒ǉ�
	 * @param[in] Component*�inew���ꂽ���̂݁j
	 * @return �R���|�[�l���gID
	 * @note �g�p���@
	 * @note �@ComponentManager::CreateComponent�o�R�ŌĂяo���B
	 * @note �A������ new Component ����B(�R���|�[�l���g�ɂ���Ă͂ł��Ȃ���������)
	 */
	int AddComponent(Component* pComponent);

private:
	/**
	* @brief �A�N�^�[id�𐶐�����
	*/
	inline int CreateActorID() {
		static int id = -1;
		id++;
		return id;
	}

public:
	/**
	 * @brief Transform�l�̎擾
	 * @return transform�l(const)
	 */
	inline const Transform& GetTransform() {
		return mTransform;
	}

	/**
	 * @brief Transform�l�̎擾
	 * @return transform�l(�ҏW�\���)
	 */
	inline Transform& GetTransformRef() {
		return mTransform;
	}

	/**
	 * @brief �R���|�[�l���g�̎擾
	 * @param[in] �R���|�[�l���gID
	 * @return Component*
	 */
	inline Component* const GetComponent(int id) {
		return mpComponents.at(id);
	}

	/**
	* @brief �A�N�^�[ID�̎擾
	*/
	inline int GetActorID() {
		return mActorID;
	}
public:

	/**
	* @brief ComponentID�z��̎擾
	*/
	inline const std::vector<int>& GetComponentIDs() {
		return mpComponentIDs;
	}

};