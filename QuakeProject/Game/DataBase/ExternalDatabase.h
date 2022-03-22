/**
 * @file ExternalDatabase.h
 * @brief �O������̒l���Ǘ�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once

#include "Game/UtilityManager/SingletonFounder.h"
#include <unordered_map>
#include <vector>
#include <xstring>

 /**
 * @brief �O���l�̃^�C�v
 */
enum class eExPathType {
	StageData,
	PlayerData,
	SoundData,
	CaterpillarData,
};

/**
* @brief �O������̒l���Ǘ�����N���X
*/
class ExternalDatabase :public SingletonFounder<ExternalDatabase> {
	friend SingletonFounder;
public:
	/** �L�[�̌^��ʖ���` */
	using KeyType = const char*;
	/** �p�X�̌^��ʖ���` */
	using PathType = const char*;

private:
	/**
	* @brief �t�@�C���p�X���[�h���ɕK�v�ȃf�[�^�Q(���P)
	*/
	struct RoadDataPack {
		std::string keyName;
		std::string pathName;
	};
	/** �t�@�C���p�X���[�h���ɕK�v�ȃf�[�^�z�� */
	std::vector<RoadDataPack> mKeepDatas;

private:

	// �t�@�C���p�X�Ǘ��z��Ŏg���I�u�W�F�N�g(���Q)
	
	/**
	* @brief �n�b�V�������I�u�W�F�N�g
	* @note ExternalDatabase��p
	*/
	struct PersonalCreateHash {
		size_t operator()(KeyType source) const
		{
			return std::hash<std::string>()(std::string(source));
		}
	};

	/**
	* @brief �L�[��r�I�u�W�F�N�g
	* @note ExternalDatabase��p
	*/
	struct PersonalEqualTo {
		bool operator()(KeyType lhs, KeyType rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

private:
	/** �O���t�@�C���̃p�X���ꊇ�Ǘ����Ă���t�@�C���ւ̃p�X */
	static const wchar_t* ALLPATHDATA_PATH;

	/** �t�@�C���p�X���ꊇ�Ǘ�����z�� */
	std::unordered_map<KeyType, PathType, PersonalCreateHash,PersonalEqualTo>
		mAllPathMap;

	/** �t�@�C���p�X�ɃA�N�Z�X����ۂɕK�v�ȃL�[���Ǘ�����z�� */
	static const std::unordered_map<eExPathType, KeyType>
		ALL_KEY;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	ExternalDatabase();

	/**
	* @brief �f�X�g���N�^
	*/
	~ExternalDatabase() = default;

public:
	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �v�f���ȊO�̃f�[�^���J���}��؂�ł��ꂼ��̕ϐ��ɐݒ肷��B
	* @param[in] buffer ������̐擪�A�h���X(�J���}��؂�̃f�[�^)
	* @param[in] format �ݒ肵�����l�̐��ɑΉ��������t�H�[�}�b�g�w��q����(�v�f�����͕s�v)
	* @param[in] args	�ݒ肵�����ϐ������̃|�C���^...
	* @note ��ԍŏ��̗v�f���u�v�f���v�̍��ł��邱�Ƃ��O��ł��B
	* @note �������ݒ肵�����ꍇ�́A�����̒����i�������j���w�肵�Ă��������B
	*/
	template<class... T>
	static void SettingOwnParamExcludingElementName(
		char const* const buffer,
		char const* const format,
		T... args
	);

public:
	/**
	 * @brief	�t�@�C���p�X���擾����
	 * @param[in] type �t�@�C���^�C�v
	 * @return	�t�@�C���p�X
	 */
	PathType GetFilePath(eExPathType type);

	/**
	 * @brief	�t�@�C����ǂݍ��݁A�f�[�^������
	 * @param[in]	path	�t�@�C���p�X
	 * @param[out]	data	�f�[�^�̓��ꕨ
	 * @return	����ɓǂݍ��߂���true,���s������false
	 * @note	���f�[�^�̓��ꕨ��wstring�^�ł���
	 */
	const bool GetDataFromPath(PathType path, std::vector<std::string>& data);
	/**
	 * @brief	�t�@�C����ǂݍ��݁A�f�[�^������
	 * @param[in]	path	�t�@�C���p�X
	 * @param[out]	data	�f�[�^�̓��ꕨ
	 * @return	����ɓǂݍ��߂���true,���s������false
	 * @note	���f�[�^�̓��ꕨ��string�^�ł���
	 */
	const bool GetDataFromPath(PathType path, std::vector<std::wstring>& data);

	/**
	 * @brief	�t�@�C����ǂݍ��݁A�f�[�^������
	 * @param[in]	path	�t�@�C���p�X
	 * @param[out]	data	�f�[�^�̓��ꕨ
	 * @return	����ɓǂݍ��߂���true,���s������false
	 */
	inline const bool GetDataFromType(eExPathType type, std::vector<std::string>& data)
	{
		// �󂯎�����^�C�v�����Ƀt�@�C���p�X�𒼂�GetDataFromPath�֐��ɓn��
		return GetDataFromPath(GetFilePath(type), data);
	}
	/**
	 * @brief	�t�@�C����ǂݍ��݁A�f�[�^������
	 * @param[in]	path	�t�@�C���p�X
	 * @param[out]	data	�f�[�^�̓��ꕨ
	 * @return	����ɓǂݍ��߂���true,���s������false
	 */
	inline const bool GetDataFromType(eExPathType type, std::vector<std::wstring>& data)
	{
		// �󂯎�����^�C�v�����Ƀt�@�C���p�X�𒼂�GetDataFromPath�֐��ɓn��
		return GetDataFromPath(GetFilePath(type), data);
	}

};

/*
	���P
	���X���̃f�[�^�����̓��[�J���I�ɐ錾���g���Ă������A
	const char* ���������p�X�����񂪉��鎖�ۂ������������߁A
	�����o�Ƃ��Ĉ����i���̖���^��������ɂ����B	
	�����Fconst char* ���������A������ɃA�N�Z�X�ł��Ȃ��Ȃ����H
	�ʈāFwstring�����Ηǂ������H

	���Q
	�ŏ��͂��̂Q������Ă��Ȃ��������A�ǂ���瓯�l���肪��肭�����Ă��炸�A
	�n�b�V����������肭�����Ă��Ȃ��Ɖ��肵�����߃n�b�V����string�������
	�s���V�X�e���ɂ����B��r�I�u�W�F�N�g���A�����I��char* == char*��������
	����̂ł�������strcmp����čs����悤���P�����B
*/

//=====================================================

/**
* @brief �v�f���ȊO�̃f�[�^���J���}��؂�ł��ꂼ��̕ϐ��ɐݒ肵�܂��B
		����ԍŏ��̗v�f���u�v�f���v�̍��ł��邱�Ƃ��O��ł��B
		���������ݒ肵�����ꍇ�́A�����̒����i�������j���w�肵�Ă��������B

* @param[in] buffer ������̐擪�A�h���X(�J���}��؂�̃f�[�^)
* @param[in] format �ݒ肵�����l�̐��ɑΉ��������t�H�[�}�b�g�w��q����(�v�f�����͕s�v)
* @param[in] args	�ݒ肵�����ϐ������̃|�C���^...
*/
template<class ...T>
static inline void ExternalDatabase::SettingOwnParamExcludingElementName(
	char const* const buffer, char const* const format, T ...args)
{
	// �t�H�[�}�b�g�w��q���u�v�f�����v���ǉ��������ɂ���
	std::string allformat = std::string("%[^,],") + std::string(format);
	char rowName[16] = {}; // �v�f���i�g��Ȃ��j
	sscanf_s(buffer, allformat.c_str(), rowName, _countof(rowName), args...);
}
