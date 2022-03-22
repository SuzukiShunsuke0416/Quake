/**
 * @file ElevationColor.h
 * @brief	�I�[�i�[�̍������W�ɉ����Ēl���ω�����
 * @brief	�F���������R���|�[�l���g
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/Component/Component.h"
#include "Libraries/MyLibraries/Utility.h"

 /**
 * @brief �I�[�i�[�̍������W�ɉ����Ēl���ω�����
 * @brief �F���������R���|�[�l���g
 * @note ElevationColorManager���ꊇ�ŕύX����
 */
class ElevationColor :public Component {
private:
	/** �F��� */
	ColorData mColor;
public:
	/**
	* @brief �R���X�g���N�^
	* @param[in] pOwner	�I�[�i�[�|�C���^
	*/
	ElevationColor(Actor* pOwner);

	/**
	* @brief �f�X�g���N�^
	*/
	~ElevationColor();

public:
	/**
	* @brief �F�����擾
	*/
	inline ColorData& GetColor() {
		return mColor;
	}

	/**
	 * @brief	�F����ݒ�
	 * @param[in] newColor �V�����F
	 */
	inline void SetColorV3(
		const DirectX::SimpleMath::Vector3& newColor)
	{
		mColor.r = newColor.x;
		mColor.g = newColor.y;
		mColor.b = newColor.z;
	}
};