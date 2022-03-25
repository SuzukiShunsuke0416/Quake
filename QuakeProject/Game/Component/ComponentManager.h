/**
 * @file ComponentManager.h
 * @brief ����R���|�[�l���g���Ǘ�����}�l�[�W���̊��N���X
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */

#pragma once
#include <vector>
#include <unordered_map>
#include "Game/Actor/Actor.h"
#include <iterator>
#include "Game/Character/CharacterActor.h"
#include "Game/UtilityManager/MemberManager.h"

 /**
 * @brief �R���|�[�l���g���Ǘ�����}�l�[�W���̊��N���X
 */
template <typename MemberType>
class ComponentManager :
	public MemberManager<MemberType>
{
protected:

public:
	/**
	* @brief �R���X�g���N�^
	*/
	ComponentManager() 
		:MemberManager()
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~ComponentManager() 
	{
	}

	/**
	* @brief ������
	*/
	virtual void Initialize() {}
	
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
	 * @fn
	 * @brief �����o�[�̎擾(actor��Component�z�񂩂��v������̂�����)
	 * @param[in] �A�N�^�[ptr
	 * @return �����o�[�̌^ptr
	 */
	MemberType* GetComponentFromActor(Actor* pActor);
};

/**
 * @fn
 * @brief �����o�[�̎擾(actor��Component�z�񂩂��v������̂�����)
 * @param �A�N�^�[ptr
 * @return �����o�[�̌^ptr
 */
template<typename MemberType>
inline MemberType* ComponentManager<MemberType>::GetComponentFromActor(Actor* pActor)
{
	// actor�������Ă���R���|�[�l���g��ID�z����擾
	const auto& ActorCompIDs = pActor->GetComponentIDs();
	// ��v����ID������z��B�i�P�v�f��������Ȃ��B�P�ȊO�Ȃ�s��j
	std::vector<int> matchIDs;
	matchIDs.reserve(1);

	// ID�z�񓯎m�ŐϏW�������߂�B
	std::set_intersection(
		mpMembersKeys.begin(), mpMembersKeys.end(),
		ActorCompIDs.begin(), ActorCompIDs.end(),
		std::back_inserter(matchIDs));

	// ����Ă���ID�����ɃR���|�[�l���g����������B
	return GetMemberFromID(matchIDs.front());
}
