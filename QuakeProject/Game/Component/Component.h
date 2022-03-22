/**
 * @file Component.h
 * @brief �u�A�N�^�ɒǉ����邱�Ƃ��ł���@�\�̍\���v�f�v�̊��N���X
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once

class Actor;
/**
* @brief �R���|�[�l���g
*/
class Component {
protected:
	/** ���̃R���|�[�l���g�̎����� */
	Actor* mpOwnerActor;

	/** �R���|�[�l���gID */
	int mComponentID;

protected:
	/**
	* @brief �R���X�g���N�^
	*/
	Component(Actor* pOwner);

public:

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~Component() = default;


	/**
	* @brief �X�V
	*/
	virtual void Update() {}

	/**
	* @brief �`��
	*/
	virtual void Render() {}

public:

	/**
	* @brief �R���|�[�l���g�h�c�擾
	*/
	int GetID() { return mComponentID; }
	
	/**
	* @brief �I�[�i�[�̎擾
	*/
	Actor* const GetOwner() { return mpOwnerActor; }

private:
	/**
	* @brief �R���|�[�l���gID����
	*/
	inline void CreateID() {
		static int id = 0;
		mComponentID = id;
		id++;
	}
};
