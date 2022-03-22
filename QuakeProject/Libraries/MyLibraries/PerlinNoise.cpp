/**
 * @file PerlinNoise.h
 * @brief �p�[�����m�C�Y�𐶐�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/11
 */
#include "pch.h"
#include "PerlinNoise.h"
#include "Utility.h"
#include <random>
#include <string>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
PerlinNoise::PerlinNoise()
	:mAdjustedValue()
{
	// �񌈒�_�I�����𐶐�
	std::random_device rnd;
	auto randVal = rnd();
	// �����̒l�����ŃV�[�h�l�Ƃ��Ďg���B�v���C�o�b�N���͂��̒l��ۑ����Ă����g���B

	// �^�������i�V�[�h�l����j�𐶐�
	std::mt19937 rand_mt(randVal);
	
	mAdjustedValue = rand_mt() * 0.0001f;
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
float PerlinNoise::operator()(const Vector2& uv)
{
	using namespace myf;

	Vector2 p = Vector2(std::floorf(uv.x), std::floorf(uv.y));
	Vector2 f = uv - p;
	Vector2 u = f * f * (Vector2(3.0f) - 2.0f * f);
	Vector2 v00 = Vector2(-1 + 2 * Random(p + Vector2(0, 0)));
	Vector2 v10 = Vector2(-1 + 2 * Random(p + Vector2(1, 0)));
	Vector2 v01 = Vector2(-1 + 2 * Random(p + Vector2(0, 1)));
	Vector2 v11 = Vector2(-1 + 2 * Random(p + Vector2(1, 1)));


	return Lerp(
		Lerp(v00.Dot(f - Vector2(0, 0)), v10.Dot(f - Vector2(1, 0)), u.x),
		Lerp(v01.Dot(f - Vector2(0, 1)), v11.Dot(f - Vector2(1, 1)), u.x),
		u.y) + 0.5f;
}

//=====================================================
//		�^����������
//=====================================================
float PerlinNoise::Random(const Vector2& uv)
{
	float ans = sinf(uv.Dot(Vector2(12.9898f, 78.233f))) * mAdjustedValue;
	return ans - int(ans);
}
