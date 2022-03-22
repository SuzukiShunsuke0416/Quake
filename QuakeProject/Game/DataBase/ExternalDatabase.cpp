#include "pch.h"
#include "ExternalDatabase.h"

#include <fstream>
#include <sstream>
#include <vector>

// 外部ファイルのパスを一括管理しているファイルへのパス
const wchar_t* ExternalDatabase::ALLPATHDATA_PATH
	= L"Resources/DataBase/AllPathData.csv";

// ファイルパスにアクセスする際に必要なキーを管理する配列
const std::unordered_map<eExPathType, ExternalDatabase::KeyType>
ExternalDatabase::ALL_KEY =
{
	{eExPathType::StageData,"Stage"},
	{eExPathType::PlayerData,"Player"},
	{eExPathType::SoundData,"Sound"},
	{eExPathType::CaterpillarData,"Cater"},
};


//=====================================================
//		コンストラクタ
//=====================================================
ExternalDatabase::ExternalDatabase()
	:mAllPathMap()
{
}

//=====================================================
//		初期化
//=====================================================
void ExternalDatabase::Initialize()
{
	// ～～～ 全てのファイルパスを取得する ～～～ //
	
	std::ifstream ifs(ALLPATHDATA_PATH);// ファイルオープン

	// １行目は要素名が入っているので飛ばす
	std::string line;
	std::getline(ifs, line, '\n');

	// １行ずつデータを読み取る
	// ※一旦仮配列(mKeepDatas)に入れ、
	// 　内容をチェックした後に本配列(mALlPathMap)に格納する。
	mKeepDatas.push_back(RoadDataPack());
	while (std::getline(ifs, line,'\n'))
	{
		std::istringstream iss(line);
		
		// １行データをカンマで分解し、キーとパス名に分ける
		std::getline(iss, mKeepDatas.back().keyName, ',');
		std::getline(iss, mKeepDatas.back().pathName, ',');

		mKeepDatas.push_back(RoadDataPack());
	}
	mKeepDatas.pop_back();


	ifs.close();// ファイルクローズ

	// ～～～ 取得したデータの「キー値」が存在するかチェック ～～～ //
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
			"外部ファイルから受け取ったキー値がプログラム側に存在しません。"
		);
	}

	// ～～～ 本配列にデータを設定する ～～～ //
	for (const auto& data : mKeepDatas) {
		mAllPathMap.emplace(data.keyName.c_str(),data.pathName.c_str());
	}

}

//=====================================================
//		ファイルパスを返す
//=====================================================
ExternalDatabase::PathType ExternalDatabase::GetFilePath(eExPathType type)
{
	return mAllPathMap.at(ALL_KEY.at(type));
}

//=====================================================
//		ファイルを読み込む
//		(入れ物がstring配列だった場合)
//=====================================================
const bool ExternalDatabase::GetDataFromPath(PathType path, std::vector<std::string>& data)
{
	std::ifstream ifs(path);// ストリーム化
	std::string line;// １行データ一時保存用

	if (!ifs) {
		assert(!"ファイルが開けません。");
		return false;
	}

	// 改行コードごとにデータを読み取る
	while (std::getline(ifs, line, '\n')) {
		data.push_back(line);
	}

	ifs.close();

	return true;
}

//=====================================================
//		ファイルを読み込む
//		(入れ物がstring配列だった場合)
//=====================================================
const bool ExternalDatabase::GetDataFromPath(PathType path, std::vector<std::wstring>& data)
{
	std::wifstream ifs(path);// ストリーム化
	std::wstring line;// １行データ一時保存用

	if (!ifs) {
		assert(!"ファイルが開けません。");
		return false;
	}

	// 改行コードごとにデータを読み取る
	while (std::getline(ifs, line)) {
		data.push_back(line);
	}

	ifs.close();

	return true;
}

/*
	パスだからwchar_t*にしたいところだったが、
	getlineが対応していないのであきらめた。
	【追記】大元を確認したら自動的に改行が区切りになっていた。
	ワイド文字列界はカンマでは無く改行区切りが当たり前なのだろうか。

	std::any_ofを使いたかったが、enum classと仲が悪いらしく
	上手くいかなかったのでラムダ式でセルフ実装した。

	eExPathType→他クラスがファイルパスを指定するときに使用
	KeyType→外部ファイルとプログラム側で、共通値にするために使用
			(enumや連番だと可読性が下がるため。)
	PathType→ファイルパス。
*/