/**
 * @file CollisionManager.h
 * @brief �����蔻����ꊇ�ŊǗ�����B
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Component/ComponentManager.h"
#include "Game/Collision/Collision.h"
#include <functional>
#include <unordered_map>

 /**
 * @brief �����蔻����ꊇ�ŊǗ�����}�l�[�W��
 */
class CollisionManager
	:public SingletonFounder<CollisionManager>
	, public ComponentManager<Collision> 
{
	friend SingletonFounder;

private:
	/** �����蔻���S������֐��������܂Ƃ߂��z�� */
	std::unordered_map<eCollisionType,
		std::function<bool(CollisionManager*, Collision*, Collision*)>>
		mCheckMap;

	/** �P�̌n�����蔻���S������֐����� */
	std::unordered_map<eCollisionType,
		std::function<void(CollisionManager*, Collision*)>>
		mSingleFunctionMap;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	CollisionManager();

	/**
	* @brief �f�X�g���N�^
	*/
	~CollisionManager() = default;

public:

	/**
	* @brief �X�V
	*/
	void Update() override;

private:
	/**
	 * @brief	����actor�����n������܂Ƃ߂�
	 * @param[in] pCol		�R���W�����|�C���^
	 * @param[in] myType	�����̃^�C�v
	 * @param[in] vsType	����̃^�C�v
	 */
	void CheckSingles(
		Collision* const pCol,
		const eCollisionType myType,
		const eCollisionType vsType);

	/**
	* @brief �L�����N�^�[�ƒn��
	*/
	void Character_vs_Floor(Collision* pCol);
};