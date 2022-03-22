/**
 * @file DebugLogManager.h
 * @brief ������\�������p�̃}�l�[�W���[
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
 * @brief �t���[�Y���O�p�f�[�^�Q
 */
struct FriezeLogData {
	char* str;
	int timer;
};

/**
 * @brief ������\�������p�̃}�l�[�W���[
 * @note	�O������l��n���ƁADraw�֐����Ă΂ꂽ��
 * @note�@�ꊇ�ŕ����`�悷��
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

	/** �����f�[�^�B */
	std::vector<wchar_t*> mOutPutLogs;
	std::vector<char*> mOutPutLogs2;
	std::vector<FriezeLogData> mOutPutLogsF;

	/** �X�v���C�g�o�b�` */
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	/** �X�v���C�g�t�H���g */
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

private:
	/**
	* @brief �R���X�g���N�^
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
	* @brief �f�X�g���N�^
	*/
	~DebugLogManager() {
		this->Finalize();
	}

	/**
	* @brief ���O�ǉ�
	* @tparam[in] data	�ǉ����郍�O�l
	* @param[in] comment	�ǉ����郍�O�l�̐���
	*/
	template<class T>
	void EntryLog(T data, wchar_t* comment);

	/**
	* @brief ���O�ǉ�
	* @param[in] comment	�ǉ����郍�O
	*/
	void EntryLog(wchar_t* comment);
	
	/**
	* @brief ���O�ǉ�
	* @param[in] comment	(string�^)
	*/
	void EntryLog(const std::string& commentStr);

	/**
	* @brief ���O�ǉ�
	* @param[in] comment	(string�^)
	* @note	���̊֐�����ǉ��������́A��莞�ԕ`�悳���B
	*/
	void EntryFriezeLog(const std::string& commentStr);

	/**
	* @brief ���O�ǉ�(�t�H�[�}�b�g���R)
	* @param[in] format	
	* @param[in] args
	* @note	printf�̂悤�Ƀt�H�[�}�b�g���w�肵�A���̌�l��ݒ肷��B
	*/
	template<class... T>
	void EntrySelfFormatLog(const char* const format, T... args);

	/**
	* @brief �`��
	*/
	void Draw();
	/**
	* @brief ���Z�b�g
	*/
	void Reset();
	/**
	* @brief �I������
	*/
	void Finalize();
};

//=====================================================
//		���O�ǉ�
//=====================================================
template<class T>
inline void DebugLogManager::EntryLog(T data, wchar_t* comment)
{
	//�󂯎�����f�[�^�^�ɉ����ďo�͕ϊ��w��q��ς���
	const wchar_t* typeData[] = 
		{
			L"%s->%d",//! int�^
			L"%s->%f",//! float�^
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
//		�t���[�Y���O�ǉ�
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
