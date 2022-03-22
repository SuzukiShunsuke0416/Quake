/**
 * @file DebugLogManager.h
 * @brief 文字を表示する専用のマネージャー
 * @author suzuki.shunsuke
 * @date ???
 */
#pragma once

#include "SingletonFounder.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <SimpleMath.h>
#include <string>
#include <vector>

/**
 * @brief フリーズログ用データ群
 */
struct FriezeLogData {
	char* str;
	int timer;
};

/**
 * @brief 文字を表示する専用のマネージャー
 * @note	外部から値を渡すと、Draw関数が呼ばれた際
 * @note　一括で文字描画する
 */
class DebugLogManager :public SingletonFounder<DebugLogManager> {
public:
	friend SingletonFounder;
private:
	static const DirectX::XMFLOAT2 RENDER_START_POS;
	static const float RENDER_DISTANCE;
	static const int STRING_LENGTH;

private:
	void Initialize();

	/** 文字データ達 */
	std::vector<wchar_t*> mOutPutLogs;
	std::vector<char*> mOutPutLogs2;
	std::vector<FriezeLogData> mOutPutLogsF;

	/** スプライトバッチ */
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	/** スプライトフォント */
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

private:
	/**
	* @brief コンストラクタ
	*/
	DebugLogManager()
		:mOutPutLogs()
		,mOutPutLogs2()
		,mOutPutLogsF()
		,mSpriteBatch()
		,mSpriteFont()
	{
		this->Initialize();
	}
public:
	/**
	* @brief デストラクタ
	*/
	~DebugLogManager() {
		this->Finalize();
	}

	/**
	* @brief ログ追加
	* @tparam[in] data	追加するログ値
	* @param[in] comment	追加するログ値の説明
	*/
	template<class T>
	void EntryLog(T data, wchar_t* comment);

	/**
	* @brief ログ追加
	* @param[in] comment	追加するログ
	*/
	void EntryLog(wchar_t* comment);
	
	/**
	* @brief ログ追加
	* @param[in] comment	(string型)
	*/
	void EntryLog(const std::string& commentStr);

	/**
	* @brief ログ追加
	* @param[in] comment	(string型)
	* @note	この関数から追加した情報は、一定時間描画される。
	*/
	void EntryFriezeLog(const std::string& commentStr);

	/**
	* @brief ログ追加(フォーマット自由)
	* @param[in] format	
	* @param[in] args
	* @note	printfのようにフォーマットを指定し、その後値を設定する。
	*/
	template<class... T>
	void EntrySelfFormatLog(const char* const format, T... args);

	/**
	* @brief 描画
	*/
	void Draw();
	/**
	* @brief リセット
	*/
	void Reset();
	/**
	* @brief 終了処理
	*/
	void Finalize();
};

//=====================================================
//		ログ追加
//=====================================================
template<class T>
inline void DebugLogManager::EntryLog(T data, wchar_t* comment)
{
	//受け取ったデータ型に応じて出力変換指定子を変える
	const wchar_t* typeData[] = 
		{
			L"%s->%d",//! int型
			L"%s->%f",//! float型
		};

	int typeNum = 0;
	
	//float ?
	if (typeid(data) == typeid(float)) {
		typeNum = 1;
	}
	//int
	else if(typeid(data) == typeid(int)){
		typeNum = 0;
	}
	else if (typeid(data) == typeid(size_t)) {
		typeNum = 0;
	}
	//other
	else {
		EntryLog(L"Miss Entry Log");
		return;
	}

	wchar_t* buf = new wchar_t[STRING_LENGTH];
	//wchar_t buf[32];
	mOutPutLogs.push_back(buf);

	swprintf_s(mOutPutLogs.back(), STRING_LENGTH, typeData[typeNum], comment, data);

}

template<class ...T>
inline void DebugLogManager::EntrySelfFormatLog(const char* const format, T ...args)
{
	char* buf = new char[STRING_LENGTH];
	mOutPutLogs2.push_back(buf);

	sprintf_s(mOutPutLogs2.back(),
		STRING_LENGTH,
		format, args...);
}

template<>
inline void DebugLogManager::EntryLog(bool flag, wchar_t* comment)
{
	wchar_t* buf = new wchar_t[STRING_LENGTH];
	mOutPutLogs.push_back(buf);

	wchar_t* flagName[] = { L"False",L"True" };

	swprintf_s(mOutPutLogs.back(),
		STRING_LENGTH,
		L"%s->%s",
		comment, flagName[int(flag)]
	);
}

template<>
inline void DebugLogManager::EntryLog(DirectX::SimpleMath::Vector2 data, wchar_t* comment)
{
	wchar_t* buf = new wchar_t[STRING_LENGTH];
	mOutPutLogs.push_back(buf);

	swprintf_s(mOutPutLogs.back(), 
		STRING_LENGTH,
		L"%s->x(%.3ff) y(%.3ff)",
		comment, data.x, data.y
	);
}

template<>
inline void DebugLogManager::EntryLog(DirectX::SimpleMath::Vector3 data, wchar_t* comment)
{
	wchar_t* buf = new wchar_t[STRING_LENGTH];
	mOutPutLogs.push_back(buf);

	swprintf_s(mOutPutLogs.back(),
		STRING_LENGTH,
		L"%s->x(%.3ff) y(%.3ff) z(%.3ff)",
		comment, data.x, data.y, data.z
	);
}

template<>
inline void DebugLogManager::EntryLog(DirectX::SimpleMath::Vector4 data, wchar_t* comment)
{
	wchar_t* buf = new wchar_t[STRING_LENGTH];
	mOutPutLogs.push_back(buf);

	swprintf_s(mOutPutLogs.back(),
		STRING_LENGTH,
		L"%s->x(%.3ff) y(%.3ff) z(%.3ff) w(%.3ff)",
		comment, data.x, data.y, data.z, data.w
	);
}

inline void DebugLogManager::EntryLog(wchar_t* data)
{
	wchar_t* buf = new wchar_t[STRING_LENGTH];
	mOutPutLogs.push_back(buf);

	swprintf_s(mOutPutLogs.back(),
		STRING_LENGTH,
		L"%s", data
	);
}

inline void DebugLogManager::EntryLog(const std::string& commentStr)
{
	char* buf = new char[STRING_LENGTH];
	mOutPutLogs2.push_back(buf);

	sprintf_s(mOutPutLogs2.back(), 
		STRING_LENGTH,
		"%s", commentStr.data());
}

//=====================================================
//		フリーズログ追加
//=====================================================
inline void DebugLogManager::EntryFriezeLog(const std::string& commentStr)
{
	if (mOutPutLogsF.size() > 64)return;

	char* buf = new char[STRING_LENGTH];
	FriezeLogData fld;
	fld.str = buf;
	fld.timer = 180;
	mOutPutLogsF.push_back(fld);

	sprintf_s(mOutPutLogsF.back().str,
		STRING_LENGTH,
		"%s", commentStr.data());
}
