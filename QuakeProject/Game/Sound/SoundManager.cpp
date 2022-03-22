/**
 * @file SoundManager.cpp
 * @brief �T�E���h�}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/03/03
 */
#include "pch.h"
#include "SoundManager.h"
#include "Game/DataBase/ExternalDatabase.h"
#include <fstream>
#include <sstream>
#include "Libraries/MyLibraries/Utility.h"

using namespace DirectX;

// SoundFadeDataPack��addVol�p�̊�{�l
const float SoundFadeDataPack::BASE_ADDVOL = 0.005f;

// �O���f�[�^�ƘA�g����ۂ́u�}�X�^�[�p�X�v�p�̃L�[��
const std::wstring SoundManager::MASTER_FILEPATH_KEY = std::wstring(L"Master");

// �L�[�^�C�v�̕ϐ���eSoundType�ɕϊ�����p�Ŏg���z��
const std::unordered_map<SoundManager::KeyType,eSoundType>
SoundManager::CONVERT_KeyType_to_eSoundType_MAP =
{
	{SoundManager::MASTER_FILEPATH_KEY, eSoundType::Master},
	
	{L"TStoPS", eSoundType::TitleToPlay},
	{L"Jump", eSoundType::Jump},
	{L"Landing0", eSoundType::Landing0},
	{L"titleBGM", eSoundType::TitleSceneBGM},
	{L"playBGM", eSoundType::PlaySceneBGM},
};

//=====================================================
//		�R���X�g���N�^
//=====================================================
SoundManager::SoundManager()
{
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
SoundManager::~SoundManager()
{
	// �G�t�F�N�g�C���X�^���X�̉��
	for (auto& soundData : mAllSoundData) {
		soundData.second.soundEffectInstance.reset();
	}
}

//=====================================================
//		������
//=====================================================
void SoundManager::Initialize()
{
	// �O���t�@�C�����特�̃p�X�����擾
	std::vector<std::wstring> exPathData;
	ExternalDatabase::GetInstance()->GetDataFromType(eExPathType::SoundData, exPathData);

	// �P�s�ڂ͗v�f���Ȃ̂ŏ���
	std::reverse(exPathData.begin(), exPathData.end());
	exPathData.pop_back();
	std::reverse(exPathData.begin(), exPathData.end());
	
	// �t�@�C���p�X���ꊇ�Ǘ�����z��
	std::unordered_map<KeyType, PathType> allFilePathMap;
	// ���ʂ��ꎞ�܂Ƃ߂Ă����z��
	std::unordered_map<KeyType, float> allSoundVolumeMap;

	// �ǂݍ��񂾃p�X�f�[�^��S�ēǂ�
	for (std::wstring& line : exPathData) {
		// �J���}�Ńf�[�^�𕪊�����
		const SplitDataType splitData = PersonalSplit(line);

		// �������ꂽ�f�[�^�����ꂼ��ϐ��ɃZ�b�g����
		KeyType keyData = std::get<0>(splitData);
		allSoundVolumeMap[keyData] = std::get<1>(splitData);
		PathType pathData = std::get<2>(splitData);
		
		// �V���ȃt�@�C���p�X�f�[�^�̐���
		allFilePathMap[keyData] = pathData;
	}

	// Masterpath�����ʂɂ���
	const PathType masterPath = allFilePathMap.at(MASTER_FILEPATH_KEY);

	// �ǂݍ��񂾃t�@�C���p�X�����ɃT�E���h��ǂݍ���
	for (auto& pathMap : allFilePathMap) {
		// �L�[�ƃp�X��ʖ���`
		const KeyType keyData = pathMap.first;
		const PathType& pathData = pathMap.second;

		// �ǂݍ��񂾃L�[���u�}�X�^�[�v�Ȃ��΂�
		if (CONVERT_KeyType_to_eSoundType_MAP.at(keyData) == eSoundType::Master) {
			continue;
		}

		// �T�E���h�f�[�^�B�̐ݒ�
		this->LoadSound(
			masterPath,	//�}�X�^�[�p�X
			keyData,	//�L�[
			pathData,	//�p�X
			allSoundVolumeMap.at(keyData)	//���ʊ���
		);
	}
}

//=====================================================
//		�X�V
//=====================================================
void SoundManager::Update()
{
	// ���݂���S�Ẳ���audioEngine���X�V����
	for (auto& soundData : mAllSoundData) {
		soundData.second.audioEngine->Update();
	}

	// �t�F�[�h�n�̏��������s����
	this->UpdataFadeSounds();
}

//=====================================================
//		�J���}�Ńf�[�^�𕪊�����iSoundManager��p�j
//=====================================================
SoundManager::SplitDataType SoundManager::PersonalSplit(const std::wstring& line)
{
	// �ŏ��̃J���}�̈ʒu��T���@��F(Test , 0.5 , test.wav�j
	size_t i = line.find(L",");
	SplitDataType data;
	//�wTest�x , 0.5 , test.wav
	std::get<0>(data) = line.substr(0, i);
	// Test , �w0.5 , test.wav�x
	std::get<2>(data) = line.substr(i + 1, line.size() - i);
	// �Q�ڂ̃J���}�̈ʒu��T���i 0.5 , test.wav�j
	i = std::get<2>(data).find(L",");
	// �w0.5�x��float�ɕϊ�
	swscanf_s(std::get<2>(data).substr(0, i).c_str(), L"%f", &std::get<1>(data));
	//  0.5 ,�wtest.wav�x
	std::get<2>(data) = std::get<2>(data).substr(i + 1, std::get<2>(data).size() - 1);
	// �J���}��؂�łR�ɕ������ꂽ�f�[�^��Ԃ�
	return data;
}

//=====================================================
//		�������[�h����
//=====================================================
void SoundManager::LoadSound(
	const PathType& masterPath,	//�}�X�^�[�p�X
	const KeyType& keyData,		//�L�[
	const PathType& pathData,	//�T�E���h�t�@�C���p�X
	const float volumeRat		//���ʊ����i0.0f�`1.0f�j
)
{
	// �I�[�f�B�I�G���W�����쐬����B
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	// �T�E���h�f�[�^�B�̐ݒ�(�Q�ƌ^�ϐ��o�R�Őݒ���s��)
	auto& soundData
		= mAllSoundData[CONVERT_KeyType_to_eSoundType_MAP.at(keyData)]
		= SoundDataPack();

	// �I�[�f�B�I�G���W���̐����A�ݒ�
	soundData.audioEngine = std::make_unique<AudioEngine>(eflags);

	// �T�E���h�G�t�F�N�g�̐����A�ݒ�
	soundData.soundEffect = std::make_unique<SoundEffect>
		(soundData.audioEngine.get(), (masterPath + pathData).c_str());

	// �T�E���h�G�t�F�N�g�C���X�^���X�̐����A�ݒ�
	soundData.soundEffectInstance = soundData.soundEffect->CreateInstance();

	// �S�̂̉��ʂ������Ă���
	soundData.audioEngine->SetMasterVolume(volumeRat);
}

//=====================================================
//		�t�F�[�h�n�T�E���h�̍X�V
//=====================================================
void SoundManager::UpdataFadeSounds()
{
	for (auto ite = mFadeSoundList.begin(); ite != mFadeSoundList.end();) {
		// ���ʊ��������Z(���Z)����
		SoundDataPack* pSoundData = ite->pSoundData;
		pSoundData->soundLerpRat += ite->addVol;
		
		// ���ʊ������ő�l�𒴂���ꍇ�͉��ʂ��ő�Ƃ��A�z�񂩂珜�O����B
		if (pSoundData->soundLerpRat >= 1.0f) {
			pSoundData->soundLerpRat = 1.0f;
			pSoundData->soundEffectInstance->SetVolume(1.0f);
			ite = mFadeSoundList.erase(ite);
			continue;
		}
		// ���ʊ������ŏ��l�������ꍇ�͉��ʂ��O�Ƃ��A�����~�߁A�z�񂩂珜�O����B
		else if (pSoundData->soundLerpRat <= 0.0f) {
			pSoundData->soundLerpRat = 0.0f;
			pSoundData->soundEffectInstance->SetVolume(0.0f);
			pSoundData->soundEffectInstance->Stop();
			ite = mFadeSoundList.erase(ite);
			continue;
		}

		// �t�F�[�h�������܂��r����������A���̂܂܉��ʂ𒲐��������B
		pSoundData->soundEffectInstance->SetVolume(pSoundData->soundLerpRat);

		++ite;
	}
}

//=====================================================
//		�t�F�[�h�������ɏ�������鎖��h�~
//=====================================================
void SoundManager::AvoidanceSimultaneousFade(const SoundDataPack* pSoundData)
{
	for (auto ite = mFadeSoundList.begin(); ite != mFadeSoundList.end();) {
		if (ite->pSoundData == pSoundData) {
			ite = mFadeSoundList.erase(ite);
			continue;
		}
		++ite;
	}
}

//=====================================================
//		�w��T�E���h���t�F�[�h�C���Ŗ炷
//=====================================================
void SoundManager::PlaySoundFadeIn(eSoundType type, bool isLoop)
{
	// �����{�����[�������߂���A�T�E���h���J�n����B
	SoundDataPack& sound = mAllSoundData.at(type);
	const float rat = sound.soundLerpRat;
	sound.soundEffectInstance->SetVolume(sound.soundLerpRat);
	sound.soundEffectInstance->Play(isLoop);

	// ���h�~
	this->AvoidanceSimultaneousFade(&sound);

	// �t�F�[�h���p�z��ɒǉ�����
	mFadeSoundList.push_back(SoundFadeDataPack(&sound, SoundFadeDataPack::BASE_ADDVOL));
}

//=====================================================
//		�w��T�E���h���t�F�[�h�A�E�g�Ŏ~�߂�
//=====================================================
void SoundManager::StopSoundFadeOut(eSoundType type)
{
	SoundDataPack& sound = mAllSoundData.at(type);

	// ���h�~
	this->AvoidanceSimultaneousFade(&sound);

	// �t�F�[�h���p�z��ɒǉ�����
	mFadeSoundList.push_back(SoundFadeDataPack(&sound, -SoundFadeDataPack::BASE_ADDVOL));
}
