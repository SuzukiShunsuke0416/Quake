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

 /**
 * @brief �R���|�[�l���g���Ǘ�����}�l�[�W���̊��N���X
 */
template <typename MemberType>
class ComponentManager {
protected:
	/**  �Ǘ����郁���o�[�����i�ʏ�g���p�j */
	std::vector<MemberType*> mpMembers;

	/** �����o�����iID�����p�j */
	std::unordered_map<int,MemberType*> mpMembersForSearch;
	
	/** �����o������ID�̂݊Ǘ����� */
	std::vector<int> mpMembersKeys;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	ComponentManager() 
		:mpMembers()
		,mpMembersForSearch()
		,mpMembersKeys()
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~ComponentManager() {
		mpMembers.clear();
		mpMembersForSearch.clear();
		mpMembersKeys.clear();
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
	 * @brief �����o�[�̒ǉ�
	 * @param[in] �ǉ�����R���|�[�l���g�̃|�C���^
	 */
	void AddMember(MemberType* pAddMem);

	/**
	 * @brief �����o�[�̏��O
	 * @param[in] ���O����R���|�[�l���g�̃|�C���^
	 */
	void RemoveMember(MemberType* pRemMem);

	/**
	 * @brief �����o�[�̎擾(ID�w��)
	 * @param[in] ComponentID
	 * @return �����o�[�̌^ptr
	 */
	MemberType* GetMemberFromID(int id);

	/**
	 * @fn
	 * @brief �����o�[�̎擾(actor��Component�z�񂩂��v������̂�����)
	 * @param[in] �A�N�^�[ptr
	 * @return �����o�[�̌^ptr
	 */
	MemberType* GetComponentFromActor(Actor* pActor);
};


//=====================================================
//		�ǉ�
//=====================================================
template<typename MemberType>
inline void ComponentManager<MemberType>::AddMember(MemberType* pAddMem)
{
	// �z��֒ǉ�����
	mpMembers.push_back(pAddMem);
	// �z��iID�����p�j�֒ǉ�����
	mpMembersForSearch[pAddMem->GetID()] = pAddMem;
	// �z��iID�j�֒ǉ�����
	mpMembersKeys.push_back(pAddMem->GetID());

	// �z��iID�j�������ɕ��ёւ���
	std::sort(mpMembersKeys.begin(), mpMembersKeys.end());
}

//=====================================================
//		���O
//=====================================================
template<typename MemberType>
inline void ComponentManager<MemberType>::RemoveMember(MemberType* pRemMem)
{
	// �z�񂩂珜�O����
	 auto result = std::remove_if(mpMembers.begin(), mpMembers.end(), [&](MemberType* mem) { return pRemMem == mem; });
	 mpMembers.erase(result, mpMembers.end());

	// �z��iID�����p�j���珜�O����
	mpMembersForSearch.erase(pRemMem->GetID());

	// �z��iID�j���珜�O����
	auto result2 = std::remove_if(mpMembersKeys.begin(), mpMembersKeys.end(), [&](int key) { return pRemMem->GetID() == key; });
	mpMembersKeys.erase(result2, mpMembersKeys.end());
}

/**
 * @fn
 * @brief �����o�[�̎擾(ID�w��)
 * @param ComponentID
 * @return �����o�[�̌^ptr
 */
template<typename MemberType>
inline MemberType* ComponentManager<MemberType>::GetMemberFromID(int id)
{
	return mpMembersForSearch.at(id);
}

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
