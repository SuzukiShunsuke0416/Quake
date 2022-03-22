/**
 * @file SoundManager.cpp
 * @brief サウンドマネージャクラス
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

// SoundFadeDataPackのaddVol用の基本値
const float SoundFadeDataPack::BASE_ADDVOL = 0.005f;

// 外部データと連携する際の「マスターパス」用のキー名
const std::wstring SoundManager::MASTER_FILEPATH_KEY = std::wstring(L"Master");

// キータイプの変数をeSoundTypeに変換する用で使う配列
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
//		コンストラクタ
//=====================================================
SoundManager::SoundManager()
{
}

//=====================================================
//		デストラクタ
//=====================================================
SoundManager::~SoundManager()
{
	// エフェクトインスタンスの解放
	for (auto& soundData : mAllSoundData) {
		soundData.second.soundEffectInstance.reset();
	}
}

//=====================================================
//		初期化
//=====================================================
void SoundManager::Initialize()
{
	// 外部ファイルから音のパス等を取得
	std::vector<std::wstring> exPathData;
	ExternalDatabase::GetInstance()->GetDataFromType(eExPathType::SoundData, exPathData);

	// １行目は要素名なので消す
	std::reverse(exPathData.begin(), exPathData.end());
	exPathData.pop_back();
	std::reverse(exPathData.begin(), exPathData.end());
	
	// ファイルパスを一括管理する配列
	std::unordered_map<KeyType, PathType> allFilePathMap;
	// 音量を一時まとめておく配列
	std::unordered_map<KeyType, float> allSoundVolumeMap;

	// 読み込んだパスデータを全て読む
	for (std::wstring& line : exPathData) {
		// カンマでデータを分割する
		const SplitDataType splitData = PersonalSplit(line);

		// 分割されたデータをそれぞれ変数にセットする
		KeyType keyData = std::get<0>(splitData);
		allSoundVolumeMap[keyData] = std::get<1>(splitData);
		PathType pathData = std::get<2>(splitData);
		
		// 新たなファイルパスデータの生成
		allFilePathMap[keyData] = pathData;
	}

	// Masterpathだけ別にする
	const PathType masterPath = allFilePathMap.at(MASTER_FILEPATH_KEY);

	// 読み込んだファイルパスを元にサウンドを読み込む
	for (auto& pathMap : allFilePathMap) {
		// キーとパスを別名定義
		const KeyType keyData = pathMap.first;
		const PathType& pathData = pathMap.second;

		// 読み込んだキーが「マスター」なら飛ばす
		if (CONVERT_KeyType_to_eSoundType_MAP.at(keyData) == eSoundType::Master) {
			continue;
		}

		// サウンドデータ達の設定
		this->LoadSound(
			masterPath,	//マスターパス
			keyData,	//キー
			pathData,	//パス
			allSoundVolumeMap.at(keyData)	//音量割合
		);
	}
}

//=====================================================
//		更新
//=====================================================
void SoundManager::Update()
{
	// 存在する全ての音のaudioEngineを更新する
	for (auto& soundData : mAllSoundData) {
		soundData.second.audioEngine->Update();
	}

	// フェード系の処理を実行する
	this->UpdataFadeSounds();
}

//=====================================================
//		カンマでデータを分割する（SoundManager専用）
//=====================================================
SoundManager::SplitDataType SoundManager::PersonalSplit(const std::wstring& line)
{
	// 最初のカンマの位置を探す　例：(Test , 0.5 , test.wav）
	size_t i = line.find(L",");
	SplitDataType data;
	//『Test』 , 0.5 , test.wav
	std::get<0>(data) = line.substr(0, i);
	// Test , 『0.5 , test.wav』
	std::get<2>(data) = line.substr(i + 1, line.size() - i);
	// ２つ目のカンマの位置を探す（ 0.5 , test.wav）
	i = std::get<2>(data).find(L",");
	// 『0.5』→floatに変換
	swscanf_s(std::get<2>(data).substr(0, i).c_str(), L"%f", &std::get<1>(data));
	//  0.5 ,『test.wav』
	std::get<2>(data) = std::get<2>(data).substr(i + 1, std::get<2>(data).size() - 1);
	// カンマ区切りで３つに分割されたデータを返す
	return data;
}

//=====================================================
//		音をロードする
//=====================================================
void SoundManager::LoadSound(
	const PathType& masterPath,	//マスターパス
	const KeyType& keyData,		//キー
	const PathType& pathData,	//サウンドファイルパス
	const float volumeRat		//音量割合（0.0f～1.0f）
)
{
	// オーディオエンジンを作成する。
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	// サウンドデータ達の設定(参照型変数経由で設定を行う)
	auto& soundData
		= mAllSoundData[CONVERT_KeyType_to_eSoundType_MAP.at(keyData)]
		= SoundDataPack();

	// オーディオエンジンの生成、設定
	soundData.audioEngine = std::make_unique<AudioEngine>(eflags);

	// サウンドエフェクトの生成、設定
	soundData.soundEffect = std::make_unique<SoundEffect>
		(soundData.audioEngine.get(), (masterPath + pathData).c_str());

	// サウンドエフェクトインスタンスの生成、設定
	soundData.soundEffectInstance = soundData.soundEffect->CreateInstance();

	// 全体の音量を下げておく
	soundData.audioEngine->SetMasterVolume(volumeRat);
}

//=====================================================
//		フェード系サウンドの更新
//=====================================================
void SoundManager::UpdataFadeSounds()
{
	for (auto ite = mFadeSoundList.begin(); ite != mFadeSoundList.end();) {
		// 音量割合を加算(減算)する
		SoundDataPack* pSoundData = ite->pSoundData;
		pSoundData->soundLerpRat += ite->addVol;
		
		// 音量割合が最大値を超える場合は音量を最大とし、配列から除外する。
		if (pSoundData->soundLerpRat >= 1.0f) {
			pSoundData->soundLerpRat = 1.0f;
			pSoundData->soundEffectInstance->SetVolume(1.0f);
			ite = mFadeSoundList.erase(ite);
			continue;
		}
		// 音量割合が最小値を下回る場合は音量を０とし、音を止め、配列から除外する。
		else if (pSoundData->soundLerpRat <= 0.0f) {
			pSoundData->soundLerpRat = 0.0f;
			pSoundData->soundEffectInstance->SetVolume(0.0f);
			pSoundData->soundEffectInstance->Stop();
			ite = mFadeSoundList.erase(ite);
			continue;
		}

		// フェード処理がまだ途中だったら、そのまま音量を調整し続投。
		pSoundData->soundEffectInstance->SetVolume(pSoundData->soundLerpRat);

		++ite;
	}
}

//=====================================================
//		フェードが同時に処理される事を防止
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
//		指定サウンドをフェードインで鳴らす
//=====================================================
void SoundManager::PlaySoundFadeIn(eSoundType type, bool isLoop)
{
	// 初期ボリュームを決めた後、サウンドを開始する。
	SoundDataPack& sound = mAllSoundData.at(type);
	const float rat = sound.soundLerpRat;
	sound.soundEffectInstance->SetVolume(sound.soundLerpRat);
	sound.soundEffectInstance->Play(isLoop);

	// 被り防止
	this->AvoidanceSimultaneousFade(&sound);

	// フェード中用配列に追加する
	mFadeSoundList.push_back(SoundFadeDataPack(&sound, SoundFadeDataPack::BASE_ADDVOL));
}

//=====================================================
//		指定サウンドをフェードアウトで止める
//=====================================================
void SoundManager::StopSoundFadeOut(eSoundType type)
{
	SoundDataPack& sound = mAllSoundData.at(type);

	// 被り防止
	this->AvoidanceSimultaneousFade(&sound);

	// フェード中用配列に追加する
	mFadeSoundList.push_back(SoundFadeDataPack(&sound, -SoundFadeDataPack::BASE_ADDVOL));
}
