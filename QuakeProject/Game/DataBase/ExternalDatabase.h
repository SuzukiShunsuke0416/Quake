/**
 * @file ExternalDatabase.h
 * @brief 外部からの値を管理するクラス
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once

#include "Game/UtilityManager/SingletonFounder.h"
#include <unordered_map>
#include <vector>
#include <xstring>

 /**
 * @brief 外部値のタイプ
 */
enum class eExPathType {
	StageData,
	PlayerData,
	SoundData,
	CaterpillarData,
};

/**
* @brief 外部からの値を管理するクラス
*/
class ExternalDatabase :public SingletonFounder<ExternalDatabase> {
	friend SingletonFounder;
public:
	/** キーの型を別名定義 */
	using KeyType = const char*;
	/** パスの型を別名定義 */
	using PathType = const char*;

private:
	/**
	* @brief ファイルパスロード時に必要なデータ群(※１)
	*/
	struct RoadDataPack {
		std::string keyName;
		std::string pathName;
	};
	/** ファイルパスロード時に必要なデータ配列 */
	std::vector<RoadDataPack> mKeepDatas;

private:

	// ファイルパス管理配列で使うオブジェクト(※２)
	
	/**
	* @brief ハッシュ生成オブジェクト
	* @note ExternalDatabase専用
	*/
	struct PersonalCreateHash {
		size_t operator()(KeyType source) const
		{
			return std::hash<std::string>()(std::string(source));
		}
	};

	/**
	* @brief キー比較オブジェクト
	* @note ExternalDatabase専用
	*/
	struct PersonalEqualTo {
		bool operator()(KeyType lhs, KeyType rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

private:
	/** 外部ファイルのパスを一括管理しているファイルへのパス */
	static const wchar_t* ALLPATHDATA_PATH;

	/** ファイルパスを一括管理する配列 */
	std::unordered_map<KeyType, PathType, PersonalCreateHash,PersonalEqualTo>
		mAllPathMap;

	/** ファイルパスにアクセスする際に必要なキーを管理する配列 */
	static const std::unordered_map<eExPathType, KeyType>
		ALL_KEY;

private:
	/**
	* @brief コンストラクタ
	*/
	ExternalDatabase();

	/**
	* @brief デストラクタ
	*/
	~ExternalDatabase() = default;

public:
	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 要素名以外のデータをカンマ区切りでそれぞれの変数に設定する。
	* @param[in] buffer 文字列の先頭アドレス(カンマ区切りのデータ)
	* @param[in] format 設定したい値の数に対応させたフォーマット指定子たち(要素名分は不要)
	* @param[in] args	設定したい変数たちのポインタ...
	* @note 一番最初の要素が「要素名」の作りであることが前提です。
	* @note 文字列を設定したい場合は、文字の長さ（文字数）も指定してください。
	*/
	template<class... T>
	static void SettingOwnParamExcludingElementName(
		char const* const buffer,
		char const* const format,
		T... args
	);

public:
	/**
	 * @brief	ファイルパスを取得する
	 * @param[in] type ファイルタイプ
	 * @return	ファイルパス
	 */
	PathType GetFilePath(eExPathType type);

	/**
	 * @brief	ファイルを読み込み、データを入れる
	 * @param[in]	path	ファイルパス
	 * @param[out]	data	データの入れ物
	 * @return	正常に読み込めたらtrue,失敗したらfalse
	 * @note	※データの入れ物はwstring型でも可
	 */
	const bool GetDataFromPath(PathType path, std::vector<std::string>& data);
	/**
	 * @brief	ファイルを読み込み、データを入れる
	 * @param[in]	path	ファイルパス
	 * @param[out]	data	データの入れ物
	 * @return	正常に読み込めたらtrue,失敗したらfalse
	 * @note	※データの入れ物はstring型でも可
	 */
	const bool GetDataFromPath(PathType path, std::vector<std::wstring>& data);

	/**
	 * @brief	ファイルを読み込み、データを入れる
	 * @param[in]	path	ファイルパス
	 * @param[out]	data	データの入れ物
	 * @return	正常に読み込めたらtrue,失敗したらfalse
	 */
	inline const bool GetDataFromType(eExPathType type, std::vector<std::string>& data)
	{
		// 受け取ったタイプを元にファイルパスを直にGetDataFromPath関数に渡す
		return GetDataFromPath(GetFilePath(type), data);
	}
	/**
	 * @brief	ファイルを読み込み、データを入れる
	 * @param[in]	path	ファイルパス
	 * @param[out]	data	データの入れ物
	 * @return	正常に読み込めたらtrue,失敗したらfalse
	 */
	inline const bool GetDataFromType(eExPathType type, std::vector<std::wstring>& data)
	{
		// 受け取ったタイプを元にファイルパスを直にGetDataFromPath関数に渡す
		return GetDataFromPath(GetFilePath(type), data);
	}

};

/*
	※１
	元々このデータたちはローカル的に宣言し使われていたが、
	const char* が解放されパス文字列が壊れる事象が発生したため、
	メンバとして扱い永遠の命を与える方式にした。	
	仮説：const char* が解放され、文字列にアクセスできなくなった？
	別案：wstringつかえば良かった？

	※２
	最初はこの２つを作っていなかったが、どうやら同値判定が上手くいっておらず、
	ハッシュ生成が上手くいっていないと仮定したためハッシュをstringを挟んで
	行うシステムにした。比較オブジェクトも、内部的にchar* == char*をやられると
	困るのでしっかりstrcmpを介して行われるよう改善した。
*/

//=====================================================

/**
* @brief 要素名以外のデータをカンマ区切りでそれぞれの変数に設定します。
		※一番最初の要素が「要素名」の作りであることが前提です。
		※文字列を設定したい場合は、文字の長さ（文字数）も指定してください。

* @param[in] buffer 文字列の先頭アドレス(カンマ区切りのデータ)
* @param[in] format 設定したい値の数に対応させたフォーマット指定子たち(要素名分は不要)
* @param[in] args	設定したい変数たちのポインタ...
*/
template<class ...T>
static inline void ExternalDatabase::SettingOwnParamExcludingElementName(
	char const* const buffer, char const* const format, T ...args)
{
	// フォーマット指定子を「要素数分」も追加した物にする
	std::string allformat = std::string("%[^,],") + std::string(format);
	char rowName[16] = {}; // 要素名（使わない）
	sscanf_s(buffer, allformat.c_str(), rowName, _countof(rowName), args...);
}
