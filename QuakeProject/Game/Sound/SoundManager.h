/**
 * @file SoundManager.h
 * @brief �T�E���h�}�l�[�W���N���X
 * @author suzuki.shunsuke
 * @date 2022/03/03
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include <Audio.h>
#include <unordered_map>
#include <xstring>
#include <list>

 /**
 * @brief ���̎��
 */
enum class eSoundType {
	Master,
	TitleToPlay,
	Jump,
	Landing0,
	TitleSceneBGM,
	PlaySceneBGM,
};

/**
* @brief �P�̉���炷�̂ɕK�v�ȃf�[�^�Q
*/
struct SoundDataPack {
	/** �I�[�f�B�I�G���W���ւ̃|�C���^ */
	std::unique_ptr<DirectX::AudioEngine> audioEngine;
	/** �T�E���h���\�[�X�̃R���e�i�N���X�ւ̃|�C���^ */
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	/** �T�E���h�G�t�F�N�g�C���X�^���X */
	std::unique_ptr<DirectX::SoundEffectInstance> soundEffectInstance;
	/** ���݂̉��ʊ���(fadein,out�Ŏg�p) */
	float soundLerpRat;

	/**
	* @brief �R���X�g���N�^
	*/
	SoundDataPack() 
		:audioEngine()
		,soundEffect()
		,soundEffectInstance()
		,soundLerpRat(0.0f)
	{
	}
};

/**
* @brief �P�̉����t�F�[�h�C���A�t�F�[�h�A�E�g�����邽�߂Ɏg���f�[�^�Q
*/
struct SoundFadeDataPack {
	/** ���ʂ�ύX����SoundDataPack��ptr */
	SoundDataPack* pSoundData;
	/** ���ʊ����ւ̉��Z�� */
	const float addVol;

	/**
	* @brief �R���X�g���N�^
	*/
	SoundFadeDataPack(SoundDataPack* pSoundData,float laddVol)
		:pSoundData(pSoundData)
		,addVol(laddVol)
	{
	}

	/** addVol�p�̊�{�l */
	static const float BASE_ADDVOL;
};

/**
* @brief ���֘A���Ǘ�����}�l�[�W��
*/
class SoundManager :public SingletonFounder<SoundManager> {
	friend SingletonFounder;
public:
	/** �L�[�̌^��ʖ���` */
	using KeyType = std::wstring;
	/** �p�X�̌^��ʖ���` */
	using PathType = std::wstring;

private:
	/** �}�X�^�[�t�@�C���p�X�p�̃L�[ */
	static const std::wstring MASTER_FILEPATH_KEY;

	/** �L�[�^�C�v�̕ϐ���eSoundType�ɕϊ�����p�Ŏg���z�� */
	static const std::unordered_map<KeyType, eSoundType>
		CONVERT_KeyType_to_eSoundType_MAP;

private:
	/** �T�E���h�f�[�^���ꊇ�Ǘ�����z�� */
	std::unordered_map<eSoundType, SoundDataPack>
		mAllSoundData;
	
	/** �t�F�[�h���̃T�E���h���܂Ƃ߂� */
	std::list<SoundFadeDataPack> mFadeSoundList;
public:
	/**
	* @brief �R���X�g���N�^
	*/
	SoundManager();

	/**
	* @brief �f�X�g���N�^
	*/
	~SoundManager();

	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	void Update();

private:
	/** ��������f�[�^�̌^ */
	using SplitDataType = std::tuple<std::wstring, float, std::wstring>;
	
	/**
	 * @brief	�J���}�Ńf�[�^�𕪊�����
	 * @param[in] line	�������镶����
	 * @return SplitDataType ������̕�����
	 */
	SplitDataType PersonalSplit(const std::wstring& line);

	/**
	 * @brief	�������[�h����
	 * @param[in] masterPath	�}�X�^�[�p�X
	 * @param[in] keyData	�L�[
	 * @param[in] pathData	�p�X
	 * @param[in] volumeRat	���ʊ���
	 */
	void LoadSound(
		const PathType& masterPath,
		const KeyType& keyData,
		const PathType& pathData,
		const float volumeRat
	);

	/**
	* @brief �t�F�[�h�n�T�E���h�̍X�V
	*/
	void UpdataFadeSounds();

	/**
	* @brief �t�F�[�h�C���ƃt�F�[�h�A�E�g�������ɏ�������鎖��h�~
	* @param[in] pSoundData	�T�E���h
	*/
	void AvoidanceSimultaneousFade(const SoundDataPack* pSoundData);

public:
	/**
	 * @brief	�T�E���h�G�t�F�N�g�C���X�^���X�̎擾
	 * @param[in] type	�T�E���h�^�C�v(enum class)
	 * @return SoundEffectInstance*	�w�肳�ꂽ�T�E���h�^�C�v�ɉ������T�E���h�f�[�^
	 */
	inline DirectX::SoundEffectInstance* GetSound(eSoundType type) {
		return mAllSoundData.at(type).soundEffectInstance.get();
	}

	/**
	 * @brief	�w��T�E���h���t�F�[�h�C���Ŗ炷
	 * @param[in] type	�T�E���h�^�C�v(enum class)
	 * @param[in] isLoop ���̃T�E���h�����[�v���邩
	 */
	void PlaySoundFadeIn(eSoundType type, bool isLoop = false);

	/**
	 * @brief	�w��T�E���h���t�F�[�h�A�E�g�Ŏ~�߂�
	 * @param[in] type	�T�E���h�^�C�v(enum class)
	 */
	void StopSoundFadeOut(eSoundType type);
};