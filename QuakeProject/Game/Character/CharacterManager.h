/**
 * @file CharacterManager.h
 * @brief キャラクター系を管理するマネージャクラス
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include <unordered_map>
#include "CharacterActor.h"
#include "Game/UtilityManager/MemberManager.h"

class Player;
 /**
 * @brief キャラクター系を管理するマネージャ
 */
class CharacterManager 
	:public SingletonFounder<CharacterManager> 
	,public MemberManager<CharacterActor>
{
	friend SingletonFounder;

private:

	/** プレイヤーだけの配列 */
	std::vector<Player*> mpPlayers;
private:
	/**
	* @brief コンストラクタ
	*/
	CharacterManager();

	/**
	* @brief デストラクタ
	*/
	~CharacterManager();

public:
	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

public:
	/**
	* @brief 全プレイヤーの取得
	*/
	inline const std::vector<Player*>& GetAllPlayers() {
		return mpPlayers;
	}

};