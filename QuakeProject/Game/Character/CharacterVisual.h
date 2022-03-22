/**
 * @file CharacterVisual.h
 * @brief �L�����N�^�[�̌����ڂ�S������N���X
 * @author suzuki.shunsuke
 * @date 2022/03/17
 */
#pragma once
#include "Libraries/MyLibraries/Transform.h"
#include "CharacterActor.h"

/**
* @brief �L�����N�^�[�̌����ڂ�S������N���X
*/
template<class T>
class CharacterVisual {
protected:
	/** �I�[�i�[��ptr */
	T* mpOwner;

	/** �L�����N�^�[�̃��f���s�� */
	DirectX::SimpleMath::Matrix mWorld;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	CharacterVisual(T* pOwner) 
		:mpOwner(pOwner)
		,mWorld()
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~CharacterVisual() = default;

	/**
	* @brief �I�[�i�[��transform�̒l�����Ƀ��[���h�s����v�Z
	*/
	inline void CalculationWorldMatrix() {
		mWorld = mpOwner->GetTransformRef().ChangeMatrix();
	}

	/**
	* @brief �I�[�i�[�|�C���^�̎擾
	*/
	inline T* const GetOwnerPtr() {
		return mpOwner;
	}
};