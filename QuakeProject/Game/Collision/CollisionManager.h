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


public:// �e�N���X�����ŌĂяo���֐������B

	/**
	 * @brief	�n�ʂƒ������ǂ��łԂ����Ă��邩���肷��
	 * @param[in] start		���̎n�_
	 * @param[in] end		���̏I�_
	 * @param[out] hitPos	�ڒn�_
	 * @param[in] rect		���ׂ�X�e�[�W�͈̔́i�v�f�ԍ��j
	 */
	static void Ground_vs_Ray(
		const DirectX::SimpleMath::Vector3& start,
		const DirectX::SimpleMath::Vector3& end,
		DirectX::SimpleMath::Vector3& hitPos,
		RECT* pRect = nullptr
	);
};