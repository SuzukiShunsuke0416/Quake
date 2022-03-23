#pragma once
#include "pch.h"
#include <SimpleMath.h>
#include <string>
#include <initializer_list>
#include <DirectXMath.h>


/********************************************************
	�y�V�F�[�_�Ŏg�p�����{�o�b�t�@�z
*********************************************************/
struct ConstBuffer
{
	DirectX::SimpleMath::Matrix		matWorld;
	DirectX::SimpleMath::Matrix		matView;
	DirectX::SimpleMath::Matrix		matProj;
	DirectX::SimpleMath::Vector4	Time;
};

const std::vector<D3D11_INPUT_ELEMENT_DESC>INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/********************************************************
	�y�o�g���f�[�^�z
	�U���͂�̗͂��܂Ƃ߂�
*********************************************************/
struct BattleData {
	int hp;
	int power;
	int maxHp;

	static const BattleData Default;

	BattleData() 
		:hp(Default.hp)
		,power(Default.power)
		,maxHp(Default.maxHp)
	{}

	BattleData(int lhp, int lpow) 
		:hp(lhp)
		,power(lpow)
		,maxHp(lhp)
	{
	}

	BattleData(int lhp, int lpow, int lmhp)
		:hp(lhp)
		, power(lpow)
		, maxHp(lmhp)
	{
	}
};

/********************************************************
	�y�����^���Q���L����^�z
*********************************************************/
template <class T>
struct Save {
	T Act;
	T Hoz;

	Save() 
		:Act()
		,Hoz()
	{}

	Save(T val) 
		:Act(val)
		,Hoz(val)
	{}

	void Reset() {
		Act = Hoz;
	}
};

/********************************************************
	�y�I��̃^�C�v�z
*********************************************************/
enum class eMenberType {
	eBatter,
	ePitcher,
	eLeftDef,
	eRightDef,

	ALL_NUM
};

/********************************************************
	�yint2�z
*********************************************************/
struct Int2 {
	int x;
	int y;

	Int2() 
		:x(0)
		,y(0)
	{
	}

	Int2(int a)
		:x(a)
		, y(a) 
	{
	}

	Int2(int lx, int ly)
		:x(lx)
		, y(ly)
	{
	}

	Int2 operator+(const Int2&);

	Int2 operator-(const Int2&);

	Int2 operator*=(const float&);

};

static bool operator==(const Int2& a, const Int2& b) {
	return (a.x == b.x && a.y == b.y);
}

static bool operator!=(const Int2& a, const Int2& b) {
	return !(a == b);
}

/********************************************************
	�y�t���O�ƃ^�C�}�[�̃Z�b�g�z
*********************************************************/
struct FlagTimer {
	bool flag;
	int timer;

	FlagTimer() 
		:flag(false)
		,timer(0)
	{
	}

	void Reset() {
		flag = false;
		timer = 0;
	}
};

/********************************************************
	�y����F���f�[�^�Q�z
*********************************************************/
struct ColorData {
	float r;
	float g;
	float b;
	float a;

private:
	DirectX::SimpleMath::Vector3 v3;
	DirectX::SimpleMath::Vector4 v4;

public:
	ColorData()
		: r(0.0f)
		, g(0.0f)
		, b(0.0f)
		, a(1.0f)
	{
	}

	ColorData(float lr, float lg, float lb, float la) 
		:r(lr)
		,g(lg)
		,b(lb)
		,a(la)
	{
	}

	ColorData(const DirectX::XMVECTORF32& xmvec32)
		: r(xmvec32.f[0])
		, g(xmvec32.f[1])
		, b(xmvec32.f[2])
		, a(xmvec32.f[3])
	{
	}

	inline const DirectX::SimpleMath::Vector3& V3() {
		v3.x = r;
		v3.y = g;
		v3.z = b;

		return v3;
	}

	inline const DirectX::SimpleMath::Vector4& V4() {
		v4.x = r;
		v4.y = g;
		v4.z = b;
		v4.w = a;

		return v4;
	}

	ColorData& operator= (const DirectX::XMVECTORF32& f32) {
		r = f32.f[0];
		g = f32.f[1];
		b = f32.f[2];
		a = f32.f[3];

		return *this;
	}
};

/********************************************************
	�y�X�v���C�g�o�b�`�ɕ`��ɕK�v�ȃf�[�^�Q�z
*********************************************************/
struct SpriteDrawDesc {
	DirectX::SimpleMath::Vector2 pos;
	RECT rect;
	float alpha;
	float rot;
	DirectX::SimpleMath::Vector2 center;
	DirectX::SimpleMath::Vector2 scale;

	SpriteDrawDesc()
		:pos()
		, rect()
		, alpha(0.0f)
		, rot()
		, center()
		, scale(DirectX::SimpleMath::Vector2::One)
	{
	}
};

/********************************************************
	�y�悭�g���֐����܂Ƃ߂����́z
*********************************************************/
namespace MyFunctions {

	//�N�����v	====================
	template<class T>
	static T Clamp(const T& val, const T& max, const T& min) {
		return std::max(std::min(val, max), min);
	}

	//�N�����v(�֐����Œl�ύX)	====================
	template<class T>
	static void ClampRef(T& val, const T& max, const T& min) {
		val = std::max(std::min(val, max), min);
	}


	//��Βl�������	====================
	class AbsoluteConverter {
	public:
		//�K�́��́u�E�Ӓl����֎~�v
		float operator=(const float& val)& {
			return fabsf(val);
		}
	};

	static AbsoluteConverter _ToAbsF_;

	//��������l�����E�ʂ�0����1�Ɏ��߂�	====================
	inline static float Change0to1F(float val, float max, float min) {
		return (val - min) / (max - min);
	}


	//0�`1�̒l�Ń��[�v������	====================
	inline static void Loop0to1(float& val) {
		if (val > 1.0f) {
			val -= 1.0f;
		}
		else if (val < 0.0f) {
			val += 1.0f;
		}
	}

	//0�`3.14�̒l�Ń��[�v������	====================
	inline static void Loop0toPI(float& val) {
		using namespace DirectX;
		if (val > XM_PI) {
			val -= XM_PI;
		}
		else if (val < XM_PI) {
			val += XM_PI;
		}
	}

	//0�`6.28�̒l�Ń��[�v������	====================
	inline static void Loop0to2PI(float& val) {
		using namespace DirectX;
		if (val > XM_2PI) {
			val -= XM_2PI;
		}
		else if (val < XM_2PI) {
			val += XM_2PI;
		}
	}

	//-2PI�`2PI�̒l�Ń��[�v������	====================
	inline static void Loop_2PIto2PI(float& val) {
		using namespace DirectX;
		if (val > XM_2PI) {
			val -= XM_2PI;
		}
		else if (val < -XM_2PI) {
			val += XM_2PI;
		}
	}

	//�f�O���[�����W�A��	====================
	inline static float ConvertToRadians(float deg) {
		return deg * (DirectX::XM_PI / 180.0f);
	}

	//���[�v	====================
	template<class T>
	inline static T Lerp(const T& start, const T& goal, float rat) {
		T difference = goal - start;
		return start + difference * rat;
	}

	// ���炩�⊮�����i����13��@�j	====================
	template <class T>
	inline static void RubberComplement(T& baseVal, const T newVal) {
		baseVal += (newVal - baseVal) * 0.07f;
	}
	// ���炩�⊮�����i���߁j	====================
	template <class T>
	inline static void RubberComplementQuick(T& baseVal, const T newVal) {
		baseVal += (newVal - baseVal) * 0.1f;
	}
	// ���炩�⊮�����i�x�߁j	====================
	template <class T>
	inline static void RubberComplementSlow(T& baseVal, const T newVal) {
		baseVal += (newVal - baseVal) * 0.04f;
	}

	/**
	 * @brief	���炩�⊮����
	 * @param[out] baseVal	���̒l
	 * @param[in] newVal �V���Ȓl
	 * @param[in] rat �⊮����
	 * @note �Q�l�l�F/13 �� 0.07f | /20 �� 0.05f | /5 �� 0.2f
	 * @note �����l�͑傫�����������ڕW�l�֓͂��܂��B
	 */
	template <class T>
	inline static void RubberComplementCustom(T& baseVal, const T newVal,const float& rat) {
		baseVal += (newVal - baseVal) * rat;
	}

	/**
	 * @brief	�p�x��p���炩�⊮����
	 * @param[out] currentAngle	���݂̒l�i�ύX�����j
	 * @param[in] orderAngle �w��̒l
	 * @param[in] rat �⊮����
	 */
	inline static void RubberComplementAngleAdjustmentCustom(
		float& currentAngle, float orderAngle, float rat = 0.1f) 
	{
		// �p�x�⊮�l�̃S�[���l�̌�� ���P
		float lmoveWayAngle[4] = { 0.0f,0.0f,0.0f,0.0f };
		lmoveWayAngle[0] = orderAngle;
		lmoveWayAngle[1] = lmoveWayAngle[0] + DirectX::XM_2PI;
		lmoveWayAngle[2] = lmoveWayAngle[0] - DirectX::XM_2PI;
		lmoveWayAngle[3] = lmoveWayAngle[0] - DirectX::XM_2PI * 2;

		// ���l�̒����猻�ݒl�ɍł��߂��l��I��
		float goalAngle = std::min(
			{ lmoveWayAngle[0], lmoveWayAngle[1], lmoveWayAngle[2], lmoveWayAngle[3] },
			[&](float lval, float rval)
			{
				return fabsf(lval - currentAngle) < fabsf(rval - currentAngle);
			}
		);

		// �ړ������i�p�x�j��ݒ�
		MyFunctions::RubberComplementCustom(currentAngle, goalAngle, rat);
		// -2pi�`2pi�̊ԂŃ��[�v������
		MyFunctions::Loop_2PIto2PI(currentAngle);
	}

	/**
	* @brief �R���p�C�����Ɂu�P����Z�v�̒l���v�Z����
	*/
	constexpr float CompileTimeDivision1byInt(const int val) {
		return 1.0f / float(val);
	}
	/**
	* @brief �R���p�C�����Ɂu�P����Z�v�̒l���v�Z����
	*/
	constexpr float CompileTimeDivision1byFloat(const float val) {
		return 1.0f / val;
	}
}

namespace myf = MyFunctions;