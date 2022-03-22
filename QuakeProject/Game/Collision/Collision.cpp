/**
 * @file Collision.cpp
 * @brief �R���W�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "Collision.h"
#include "CollisionManager.h"

 //=====================================================
 //		�R���X�g���N�^
 //=====================================================
Collision::Collision(
	Actor* pOwner, 
	eCollisionType myType, 
	eCollisionType vsType, 
	HitFuncType hitFunctionPtr
)
	: Component(pOwner)
	, mMyType(myType)
	, mVsType(vsType)
	, mHitFunction(hitFunctionPtr)
{
	// �ǉ�
	CollisionManager::GetInstance()->AddMember(this);

	// hit�֐�ptr��null�`�F�b�N
	if (mHitFunction == nullptr) {
		// �֐�ptr���ݒ肳��Ă��Ȃ��ꍇ�́A�K���ȃ����_����}�����Ă����B
		mHitFunction = [](Collision* pCol) { UNREFERENCED_PARAMETER(pCol); };
	}
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
Collision::~Collision()
{
	// ���O
	CollisionManager::GetInstance()->RemoveMember(this);
}

//=====================================================
//		 Hit�֐�(�����o�R�Ŋe�A�N�^�[��Hit�֐��֔��)
//=====================================================
void Collision::Hit(Collision* pCol)
{
	mHitFunction(pCol);
}


/*============================================
	eCollisionType�p�̉��Z�q�I�[�o�[���[�h
============================================*/
eCollisionType operator&(const eCollisionType& t1, const eCollisionType& t2)
{
	return static_cast<eCollisionType>
		(static_cast<unsigned>(t1) & static_cast<unsigned>(t2));
}

eCollisionType operator|(const eCollisionType& t1, const eCollisionType& t2)
{
	return static_cast<eCollisionType>
		(static_cast<unsigned>(t1) | static_cast<unsigned>(t2));
}
