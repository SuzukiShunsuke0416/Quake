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
	:mpCharacters()
{
}

//=====================================================
//		デストラクタ
//=====================================================
CharacterManager::~CharacterManager()
{
	mpCharacters.clear();
}

//=====================================================
//		初期化
//=====================================================
void CharacterManager::Initialize()
{
	// プレイヤーの生成
	this->AddCharacter(std::make_unique<Player>());
}

//=====================================================
//		更新
//=====================================================
void CharacterManager::Update()
{
	// キャラクターたちの更新
	for (const auto& chara : mpCharacters) {
		chara.second->Update();
	}
}

//=====================================================
//		描画
//=====================================================
void CharacterManager::Render()
{
	// キャラクターたちの描画
	for (const auto& chara : mpCharacters) {
		chara.second->Render();
	}
}

//=====================================================
//		キャラの追加
//=====================================================
void CharacterManager::AddCharacter(std::unique_ptr<CharacterActor> character)
{
	int id = character->GetActorID();
	mpCharacters[id] = std::move(character);
}
