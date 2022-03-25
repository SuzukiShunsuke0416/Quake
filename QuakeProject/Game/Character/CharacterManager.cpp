/**
 * @file CharacterManager.cpp
 * @brief キャラクター系を管理するマネージャクラス
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#include "pch.h"
#include "CharacterManager.h"
#include "Player/Player.h"

 //=====================================================
 //		コンストラクタ
 //=====================================================
CharacterManager::CharacterManager()
	:mpPlayers()
{
}

//=====================================================
//		デストラクタ
//=====================================================
CharacterManager::~CharacterManager()
{
	for (auto& chara : mpMembers) {
		delete chara;
		chara = nullptr;
	}
}

//=====================================================
//		初期化
//=====================================================
void CharacterManager::Initialize()
{
	// プレイヤーの生成
	mpPlayers.push_back(new Player());

	// プレイヤーの登録
	for (const auto& pla : mpPlayers) {
		this->AddMember(pla);
	}
	
}

//=====================================================
//		更新
//=====================================================
void CharacterManager::Update()
{
	// キャラクターたちの更新
	for (const auto& chara : mpMembers) {
		chara->Update();
	}
}

//=====================================================
//		描画
//=====================================================
void CharacterManager::Render()
{
	// キャラクターたちの描画
	for (const auto& chara : mpMembers) {
		chara->Render();
	}
}
