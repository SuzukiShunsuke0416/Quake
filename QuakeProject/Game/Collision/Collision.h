/**
 * @file Collision.h
 * @brief �R���W�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/Component/Component.h"
#include <functional>


/**
* @brief �����蔻��̎�ޓ���Ɏg���^
*/
enum class eCollisionType : unsigned{
	NONE = 0,

	//===�e�A�N�^�[�ɉ����ă^�C�v�����===//
	Player	= 1 << 0,
	Enemy	= 1 << 1,
	Item	= 1 << 2,

	//===�e�A�N�^�[���P�g�Ŕ�������ɍs���^�C�v===//
	Floor	= 1 << 3,
	Wall	= 1 << 4,

	SINGLE_START = Floor,
	SINGLE_END = Wall,

	//===����actor�����Ȃ��Ă�������������^�Q===//
	SINGLES = (Floor | Wall),

};

/**
* @brief eCollisionType�������₷������֐����܂Ƃ߂����O���
*/
namespace ctf {
	/**
	 * @brief	�󂯎�������̃R���W�����^�C�v��Ԃ�
	 * @param[in] type	���݂̃R���W�����^�C�v
	 * @return eCollisionType ���̃R���W�����^�C�v
	 */
	static eCollisionType next(const eCollisionType type) 
	{
		return static_cast<eCollisionType>(static_cast<unsigned>(type) * 2);
	}
}

/*============================================
	eCollisionType�p�̉��Z�q�I�[�o�[���[�h
============================================*/

/** eCollisionType���Z�q�@�� */
eCollisionType operator&(const eCollisionType& t1, const eCollisionType& t2);

/** eCollisionType���Z�q�@| */
eCollisionType operator|(const eCollisionType& t1, const eCollisionType& t2);


/**
* @brief �R���W�����N���X
*/
class Collision :public Component {
public:
	/** �����蔻������֐��̌^��ʖ���` */
	using HitFuncType = std::function<void(Collision*)>;

private:
	/** �����̃^�C�v */
	eCollisionType mMyType;

	/** ����̃^�C�v */
	eCollisionType mVsType;

	// �I�[�i�[ptr
	//(component�N���X�ɗL)

	/** �I�[�i�[��Hit�֐��̃|�C���^ */
	HitFuncType mHitFunction;

public:

	/**
	 * @fn
	 * @brief	Collision�N���X�̃R���X�g���N�^
	 * @param[in] pOwner	�I�[�i�[�|�C���^
	 * @param[in] myType	�����̃^�C�v
	 * @param[in] vsType	����̃^�C�v
	 * @param[in] hitFunctionPtr	�q�b�g�֐��Ăяo���p�֐�ptr
	 */
	Collision(
		Actor* pOwner, 
		eCollisionType myType, 
		eCollisionType vsType, 
		HitFuncType hitFunctionPtr
	);

	/**
	* @brief �f�X�g���N�^
	*/
	~Collision();

public:

	/**
	 * @brief	Hit�֐�
	 * @param[in] pCol	�R���W�����|�C���^
	 * @note	(�����o�R�Ŋe�A�N�^�[��Hit�֐��֔��)
	 */
	void Hit(Collision* pCol);

public:
	/**
	* @brief �����̃^�C�v���擾
	*/
	eCollisionType GetMyType() { return mMyType; }

	/**
	* @brief ����̃^�C�v���擾
	*/
	eCollisionType GetVsType() { return mVsType; }
};