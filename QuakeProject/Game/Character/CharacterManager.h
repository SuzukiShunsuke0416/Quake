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

class Player;
 /**
 * @brief キャラクター系を管理するマネージャ
 */
class CharacterManager :public SingletonFounder<CharacterManager> {
	friend SingletonFounder;

private:
	/** キャラクターたち（大元データ） */
	std::unordered_map<int,std::unique_ptr<CharacterActor>> 
		mpCharacters;

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
	 * @brief	キャラクターの追加
	 * @param[in] character	CharacterActorのユニークポインタ
	 */
	void AddCharacter(std::unique_ptr<CharacterActor> character);

	/**
	* @brief キャラクターの取得
	*/
	inline CharacterActor* GetCharacter(int id) {
		return mpCharacters.at(id).get();
	}
};