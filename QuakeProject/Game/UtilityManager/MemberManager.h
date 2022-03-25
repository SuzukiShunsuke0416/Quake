/**
 * @file MemberManager.h
 * @brief ����N���X���ꊇ�Ǘ�����}�l�[�W���N���X�̊��
 * @author suzuki.shunsuke
 * @date 2022/03/26
 */
#pragma once

 /**
 * @brief ����N���X���ꊇ�Ǘ�����}�l�[�W���N���X�̊��
 * @note �����o�Ƃ��Ĉ�����^�́A��ӂɊ���U��ꂽID(int�^)�������̂̂݁B
 *		 ������ID���uGetID�v�Ƃ����֐��Ŏ擾�ł���悤�ɂ��Ă����B
 *		 �Ȃ��A�����o�B�̃|�C���^�͊Ǘ����邪�������Ǘ��͒S�����Ȃ��B
 */
template <typename MemberType>
class MemberManager {
protected:
	/**  �Ǘ����郁���o�[�����i�ʏ�g���p�j */
	std::vector<MemberType*> mpMembers;

	/** �����o�����iID�����p�j */
	std::unordered_map<int, MemberType*> mpMembersForSearch;

	/** �����o������ID�̂݊Ǘ����� */
	std::vector<int> mpMembersKeys;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	MemberManager()
		:mpMembers()
		, mpMembersForSearch()
		, mpMembersKeys()
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~MemberManager() {
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
	 * @return �ǉ����������o��ID
	 */
	int AddMember(MemberType* pAddMem);

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
};


//=====================================================
//		�ǉ�
//=====================================================
template<typename MemberType>
inline int MemberManager<MemberType>::AddMember(MemberType* pAddMem)
{
	int id = pAddMem->GetID();

	// �z��֒ǉ�����
	mpMembers.push_back(pAddMem);
	// �z��iID�����p�j�֒ǉ�����
	mpMembersForSearch[id] = pAddMem;
	// �z��iID�j�֒ǉ�����
	mpMembersKeys.push_back(id);

	// �z��iID�j�������ɕ��ёւ���
	std::sort(mpMembersKeys.begin(), mpMembersKeys.end());

	// ID��ԋp����
	return id;
}

//=====================================================
//		���O
//=====================================================
template<typename MemberType>
inline void MemberManager<MemberType>::RemoveMember(MemberType* pRemMem)
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
inline MemberType* MemberManager<MemberType>::GetMemberFromID(int id)
{
	return mpMembersForSearch.at(id);
}