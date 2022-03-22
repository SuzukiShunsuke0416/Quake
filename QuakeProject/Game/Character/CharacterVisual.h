/**
 * @file CharacterVisual.h
 * @brief キャラクターの見た目を担当するクラス
 * @author suzuki.shunsuke
 * @date 2022/03/17
 */
#pragma once
#include "Libraries/MyLibraries/Transform.h"
#include "CharacterActor.h"

/**
* @brief キャラクターの見た目を担当するクラス
*/
template<class T>
class CharacterVisual {
protected:
	/** オーナーのptr */
	T* mpOwner;

	/** キャラクターのモデル行列 */
	DirectX::SimpleMath::Matrix mWorld;

public:
	/**
	* @brief コンストラクタ
	*/
	CharacterVisual(T* pOwner) 
		:mpOwner(pOwner)
		,mWorld()
	{
	}

	/**
	* @brief デストラクタ
	*/
	virtual ~CharacterVisual() = default;

	/**
	* @brief オーナーのtransformの値を元にワールド行列を計算
	*/
	inline void CalculationWorldMatrix() {
		mWorld = mpOwner->GetTransformRef().ChangeMatrix();
	}

	/**
	* @brief オーナーポインタの取得
	*/
	inline T* const GetOwnerPtr() {
		return mpOwner;
	}
};