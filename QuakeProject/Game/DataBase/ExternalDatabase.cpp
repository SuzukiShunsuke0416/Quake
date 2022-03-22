#include "pch.h"
#include "ExternalDatabase.h"

#include <fstream>
#include <sstream>
#include <vector>

// �O���t�@�C���̃p�X���ꊇ�Ǘ����Ă���t�@�C���ւ̃p�X
const wchar_t* ExternalDatabase::ALLPATHDATA_PATH
	= L"Resources/DataBase/AllPathData.csv";

// �t�@�C���p�X�ɃA�N�Z�X����ۂɕK�v�ȃL�[���Ǘ�����z��
const std::unordered_map<eExPathType, ExternalDatabase::KeyType>
ExternalDatabase::ALL_KEY =
{
	{eExPathType::StageData,"Stage"},
	{eExPathType::PlayerData,"Player"},
	{eExPathType::SoundData,"Sound"},
	{eExPathType::CaterpillarData,"Cater"},
};


//=====================================================
//		�R���X�g���N�^
//=====================================================
ExternalDatabase::ExternalDatabase()
	:mAllPathMap()
{
}

//=====================================================
//		������
//=====================================================
void ExternalDatabase::Initialize()
{
	// �`�`�` �S�Ẵt�@�C���p�X���擾���� �`�`�` //
	
	std::ifstream ifs(ALLPATHDATA_PATH);// �t�@�C���I�[�v��

	// �P�s�ڂ͗v�f���������Ă���̂Ŕ�΂�
	std::string line;
	std::getline(ifs, line, '\n');

	// �P�s���f�[�^��ǂݎ��
	// ����U���z��(mKeepDatas)�ɓ���A
	// �@���e���`�F�b�N������ɖ{�z��(mALlPathMap)�Ɋi�[����B
	mKeepDatas.push_back(RoadDataPack());
	while (std::getline(ifs, line,'\n'))
	{
		std::istringstream iss(line);
		
		// �P�s�f�[�^���J���}�ŕ������A�L�[�ƃp�X���ɕ�����
		std::getline(iss, mKeepDatas.back().keyName, ',');
		std::getline(iss, mKeepDatas.back().pathName, ',');

		mKeepDatas.push_back(RoadDataPack());
	}
	mKeepDatas.pop_back();


	ifs.close();// �t�@�C���N���[�Y

	// �`�`�` �擾�����f�[�^�́u�L�[�l�v�����݂��邩�`�F�b�N �`�`�` //
	auto any_of = [&](const RoadDataPack& data)
	{
		for (auto ite = ALL_KEY.begin(); ite != ALL_KEY.end(); ++ite) {
			if (strcmp((*ite).second, data.keyName.c_str()) == 0) {
				return true;
			}
		}
		return false;
	};
	for (const auto& data : mKeepDatas) {
		assert(
			any_of(data) &&
			"�O���t�@�C������󂯎�����L�[�l���v���O�������ɑ��݂��܂���B"
		);
	}

	// �`�`�` �{�z��Ƀf�[�^��ݒ肷�� �`�`�` //
	for (const auto& data : mKeepDatas) {
		mAllPathMap.emplace(data.keyName.c_str(),data.pathName.c_str());
	}

}

//=====================================================
//		�t�@�C���p�X��Ԃ�
//=====================================================
ExternalDatabase::PathType ExternalDatabase::GetFilePath(eExPathType type)
{
	return mAllPathMap.at(ALL_KEY.at(type));
}

//=====================================================
//		�t�@�C����ǂݍ���
//		(���ꕨ��string�z�񂾂����ꍇ)
//=====================================================
const bool ExternalDatabase::GetDataFromPath(PathType path, std::vector<std::string>& data)
{
	std::ifstream ifs(path);// �X�g���[����
	std::string line;// �P�s�f�[�^�ꎞ�ۑ��p

	if (!ifs) {
		assert(!"�t�@�C�����J���܂���B");
		return false;
	}

	// ���s�R�[�h���ƂɃf�[�^��ǂݎ��
	while (std::getline(ifs, line, '\n')) {
		data.push_back(line);
	}

	ifs.close();

	return true;
}

//=====================================================
//		�t�@�C����ǂݍ���
//		(���ꕨ��string�z�񂾂����ꍇ)
//=====================================================
const bool ExternalDatabase::GetDataFromPath(PathType path, std::vector<std::wstring>& data)
{
	std::wifstream ifs(path);// �X�g���[����
	std::wstring line;// �P�s�f�[�^�ꎞ�ۑ��p

	if (!ifs) {
		assert(!"�t�@�C�����J���܂���B");
		return false;
	}

	// ���s�R�[�h���ƂɃf�[�^��ǂݎ��
	while (std::getline(ifs, line)) {
		data.push_back(line);
	}

	ifs.close();

	return true;
}

/*
	�p�X������wchar_t*�ɂ������Ƃ��낾�������A
	getline���Ή����Ă��Ȃ��̂ł�����߂��B
	�y�ǋL�z�匳���m�F�����玩���I�ɉ��s����؂�ɂȂ��Ă����B
	���C�h������E�̓J���}�ł͖������s��؂肪������O�Ȃ̂��낤���B

	std::any_of���g�������������Aenum class�ƒ��������炵��
	��肭�����Ȃ������̂Ń����_���ŃZ���t���������B

	eExPathType�����N���X���t�@�C���p�X���w�肷��Ƃ��Ɏg�p
	KeyType���O���t�@�C���ƃv���O�������ŁA���ʒl�ɂ��邽�߂Ɏg�p
			(enum��A�Ԃ��Ɖǐ��������邽�߁B)
	PathType���t�@�C���p�X�B
*/