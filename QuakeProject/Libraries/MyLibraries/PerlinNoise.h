/**
 * @file PerlinNoise.h
 * @brief �p�[�����m�C�Y�𐶐�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/11
 */
#pragma once

 /**
 * @brief �p�[�����m�C�Y�𐶐�����N���X
 */
class PerlinNoise {
private:
	/** ���������l */
	float mAdjustedValue;

public:
	/**
	* @brief �R���X�g���N�^
	*/
	PerlinNoise();

	/**
	* @brief �f�X�g���N�^
	*/
	~PerlinNoise() = default;

	/**
	 * @brief	�p�[�����m�C�Y�̌��ʂ��擾
	 */
	float operator()(const DirectX::SimpleMath::Vector2& uv);

private:
	/**
	 * @brief	�^����������
	 */
	float Random(const DirectX::SimpleMath::Vector2& uv);
};