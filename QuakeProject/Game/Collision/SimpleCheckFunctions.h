/**
 * @file SimpleCheckFunctions.h
 * @brief �V���v���Ȕ�����s���֐����܂Ƃ߂��t�@�C��
 * @author suzuki.shunsuke
 * @date 2022/02/06
 */
#pragma once
#include "pch.h"

 /**
 * @brief �l�p�^�̏����܂Ƃ߂Ă����f�[�^�Q
 */
template <class T>
struct BoxParam {
	T left;
	T right;
	T top;
	T bottom;

	/**
	* @brief �R���X�g���N�^
	*/
	BoxParam(T lleft, T lright, T ltop, T lbottom) 
		:left(lleft)
		,right(lright)
		,top(ltop)
		,bottom(lbottom)
	{
	}

	/**
	* @brief �����k�܂���B
	* @note left�ɂ͑����Aright�ɂ͈����Btop,bottom�������B
	*/
	void Shrink(T x, T y) {
		left += x; right -= x;
		top += y; bottom -= y;
	}

	/**
	* @brief ���������L�΂��B
	* @note left�ɂ͈����Aright�ɂ͑����Btop,bottom�������B
	*/
	void Grow(T x, T y) {
		left -= x; right += x;
		top -= y; bottom += y;
	}
};

/**
* @brief �V���v���Ȕ�����s���֐��𖼑O���
*/
namespace SimpleCheckFunctions {

	/**
	 * @brief	�㉺���l(�l�p)�̒��ɒl(�_)�������Ă��邩�ǂ���
	 * @tparam[in] left		�l�p�̍�
	 * @tparam[in] invalX	�_�̉����W
	 * @tparam[in] right	�l�p�̉E
	 * @tparam[in] top		�l�p�̏�
	 * @tparam[in] inValY	�_�̏c���W
	 * @tparam[in] bottom	�l�p�̉�
	 * @return bool	
	 */
	template <class T>
	inline static bool IsBoxInPoint(T left, T invalX, T right, T top, T inValY, T bottom) {
		return
			(left <= invalX && invalX <= right) &&
			(top <= inValY && inValY <= bottom);
	}

	/**
	 * @brief	�㉺���l(�l�p)�̒��ɒl(�_)�������Ă��邩�ǂ���
	 * @param[in] outBox	�l�p�̃f�[�^�Q
	 * @tparam[in] invalX	�_�̉����W
	 * @tparam[in] inValY	�_�̏c���W
	 * @return bool
	 */
	template <class T>
	inline static bool IsBoxInPoint(const BoxParam<T>& outBox, const T& inValX, const T& inValY) {
		return IsBoxInPoint(outBox.left, inValX, outBox.right, outBox.top, inValY, outBox.bottom);
	}

	/**
	 * @brief	�㉺���l(�l�p)�̒��ɒl(�_)�������Ă��邩�ǂ���
	 * @param[in] outBox	�l�p�̃f�[�^�Q
	 * @tparam[out] invalX	�_�̉����W
	 * @tparam[out] inValY	�_�̏c���W
	 * @note �������l�p����_���o�Ă����ꍇ�A�󂯎�����_�̍��W��ύX����B
	 */
	template <class T>
	inline static void IsBoxInPointRef(const BoxParam<T>& outBox, T& inValX, T& inValY) {
		myf::ClampRef(inValX, outBox.right, outBox.left);
		myf::ClampRef(inValY, outBox.bottom, outBox.top);
	}

	/**
	 * @brief	�㉺���l(�l�p)�̒��ɒl(�_)�������Ă��邩�ǂ���(vec2�o�[�W����)
	 * @param[in] topleft		����
	 * @param[in] bottomright	�E��
	 * @param[in] inVal			�_
	 * @return bool
	 */
	inline static bool IsBoxInPoint(
		const DirectX::SimpleMath::Vector2& topleft,
		const DirectX::SimpleMath::Vector2& bottomright,
		const DirectX::SimpleMath::Vector2& inVal) 
	{
		return 
			(topleft.x <= inVal.x && inVal.x <= bottomright.x) &&
			(topleft.y <= inVal.y && inVal.y <= bottomright.y);
	}

	/**
	 * @brief	�㉺���l�̒��ɒl(��)�������Ă��邩�ǂ���
	 * @param[in] outBox	�O����
	 * @param[in] inBox		������
	 * @return bool
	 */
	template <class T>
	inline static bool IsBoxInBox(const BoxParam<T>& outBox, const BoxParam<T>& inBox) {
		return
			(outBox.left <= inBox.left && inBox.right <= outBox.right) &&
			(outBox.top <= inBox.top && inBox.bottom <= outBox.bottom);
	}
}