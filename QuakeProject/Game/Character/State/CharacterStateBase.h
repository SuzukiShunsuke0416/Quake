/**
 * @file CharacterStateBase.h
 * @brief キャラクターステート
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#pragma once

class CharacterActor;

/**
* @brief キャラクターステートの基底クラス
*/
class CharacterStateBase {
protected:
	/** ステートの持ち主 */
	CharacterActor* mpOwner;

public:
	/**
	* @brief コンストラクタ
	*/
	CharacterStateBase(CharacterActor* pOwner)
		:mpOwner(pOwner)
	{
	}

	/**
	* @brief デストラクタ
	*/
	virtual ~CharacterStateBase() = default;

	/**
	* @brief リセット
	*/
	virtual void Reset() = 0;

	/**
	* @brief 更新
	*/
	virtual void Update() = 0;

	/**
	* @brief 終了処理
	*/
	virtual void Finalize() = 0;
};