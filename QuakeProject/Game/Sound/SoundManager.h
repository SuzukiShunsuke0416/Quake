/**
 * @file SoundManager.h
 * @brief サウンドマネージャクラス
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
 * @brief 音の種類
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
* @brief １つの音を鳴らすのに必要なデータ群
*/
struct SoundDataPack {
	/** オーディオエンジンへのポインタ */
	std::unique_ptr<DirectX::AudioEngine> audioEngine;
	/** サウンドリソースのコンテナクラスへのポインタ */
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	/** サウンドエフェクトインスタンス */
	std::unique_ptr<DirectX::SoundEffectInstance> soundEffectInstance;
	/** 現在の音量割合(fadein,outで使用) */
	float soundLerpRat;

	/**
	* @brief コンストラクタ
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
* @brief １つの音をフェードイン、フェードアウトさせるために使うデータ群
*/
struct SoundFadeDataPack {
	/** 音量を変更するSoundDataPackのptr */
	SoundDataPack* pSoundData;
	/** 音量割合への加算量 */
	const float addVol;

	/**
	* @brief コンストラクタ
	*/
	SoundFadeDataPack(SoundDataPack* pSoundData,float laddVol)
		:pSoundData(pSoundData)
		,addVol(laddVol)
	{
	}

	/** addVol用の基本値 */
	static const float BASE_ADDVOL;
};

/**
* @brief 音関連を管理するマネージャ
*/
class SoundManager :public SingletonFounder<SoundManager> {
	friend SingletonFounder;
public:
	/** キーの型を別名定義 */
	using KeyType = std::wstring;
	/** パスの型を別名定義 */
	using PathType = std::wstring;

private:
	/** マスターファイルパス用のキー */
	static const std::wstring MASTER_FILEPATH_KEY;

	/** キータイプの変数をeSoundTypeに変換する用で使う配列 */
	static const std::unordered_map<KeyType, eSoundType>
		CONVERT_KeyType_to_eSoundType_MAP;

private:
	/** サウンドデータを一括管理する配列 */
	std::unordered_map<eSoundType, SoundDataPack>
		mAllSoundData;
	
	/** フェード中のサウンドをまとめる */
	std::list<SoundFadeDataPack> mFadeSoundList;
public:
	/**
	* @brief コンストラクタ
	*/
	SoundManager();

	/**
	* @brief デストラクタ
	*/
	~SoundManager();

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update();

private:
	/** 分割するデータの型 */
	using SplitDataType = std::tuple<std::wstring, float, std::wstring>;
	
	/**
	 * @brief	カンマでデータを分割する
	 * @param[in] line	分割する文字列
	 * @return SplitDataType 分割後の文字列
	 */
	SplitDataType PersonalSplit(const std::wstring& line);

	/**
	 * @brief	音をロードする
	 * @param[in] masterPath	マスターパス
	 * @param[in] keyData	キー
	 * @param[in] pathData	パス
	 * @param[in] volumeRat	音量割合
	 */
	void LoadSound(
		const PathType& masterPath,
		const KeyType& keyData,
		const PathType& pathData,
		const float volumeRat
	);

	/**
	* @brief フェード系サウンドの更新
	*/
	void UpdataFadeSounds();

	/**
	* @brief フェードインとフェードアウトが同時に処理される事を防止
	* @param[in] pSoundData	サウンド
	*/
	void AvoidanceSimultaneousFade(const SoundDataPack* pSoundData);

public:
	/**
	 * @brief	サウンドエフェクトインスタンスの取得
	 * @param[in] type	サウンドタイプ(enum class)
	 * @return SoundEffectInstance*	指定されたサウンドタイプに応じたサウンドデータ
	 */
	inline DirectX::SoundEffectInstance* GetSound(eSoundType type) {
		return mAllSoundData.at(type).soundEffectInstance.get();
	}

	/**
	 * @brief	指定サウンドをフェードインで鳴らす
	 * @param[in] type	サウンドタイプ(enum class)
	 * @param[in] isLoop そのサウンドがループするか
	 */
	void PlaySoundFadeIn(eSoundType type, bool isLoop = false);

	/**
	 * @brief	指定サウンドをフェードアウトで止める
	 * @param[in] type	サウンドタイプ(enum class)
	 */
	void StopSoundFadeOut(eSoundType type);
};