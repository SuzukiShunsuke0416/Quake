#pragma once
#include "pch.h"
#include <SimpleMath.h>

/********************************************************
	【座標・回転・拡縮をまとめた構造体】
*********************************************************/
struct Transform {
	DirectX::SimpleMath::Vector3 Location;
	DirectX::SimpleMath::Vector3 Rotation;
	DirectX::SimpleMath::Vector3 Scale;

	DirectX::SimpleMath::Matrix matrix;
	DirectX::SimpleMath::Vector2 LocationFromSky;
public:
	Transform()
		: Location(DirectX::SimpleMath::Vector3::Zero)
		, Rotation(DirectX::SimpleMath::Vector3::Zero)
		, Scale(DirectX::SimpleMath::Vector3::One)
		, matrix(DirectX::SimpleMath::Matrix::Identity)
		, LocationFromSky(DirectX::SimpleMath::Vector2::Zero)
	{
	}

	Transform(DirectX::SimpleMath::Vector3 loc, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Vector3 sca)
		: Location(loc)
		, Rotation(rot)
		, Scale(sca)
		, matrix(DirectX::SimpleMath::Matrix::Identity)
		, LocationFromSky(DirectX::SimpleMath::Vector2::Zero)
	{
	}

	void AddLocation(DirectX::SimpleMath::Vector3 addLocation);

	void AddRotation(DirectX::SimpleMath::Vector3 addRot);

	const DirectX::SimpleMath::Matrix& ChangeMatrix();

	const DirectX::SimpleMath::Matrix& ChangeMatrix_IgnoreScale();

	const DirectX::SimpleMath::Vector2& ChangeLocationFromSky();

	static const Transform Default;

	Transform operator+= (const Transform& trans) {
		Location += trans.Location;
		Rotation += trans.Rotation;
		Scale += trans.Scale;
		return *this;
	}

	Transform operator- (const Transform& trans) {
		Transform ans;
		ans.Location = Location - trans.Location;
		ans.Rotation = Rotation - trans.Rotation;
		ans.Scale = Scale - trans.Scale;
		return ans;
	}

	Transform operator* (const float f) {
		Transform ans;
		ans.Location = Location * f;
		ans.Rotation = Rotation * f;
		ans.Scale = Scale * f;
		return ans;
	}

	Transform operator/ (const float f) {
		Transform ans;
		ans.Location = Location / f;
		ans.Rotation = Rotation / f;
		ans.Scale = Scale / f;
		return ans;
	}
};

/********************************************************
	【Transformクラスを使いやすくするヘルパー関数群】
*********************************************************/
class Actor;
namespace TransformHelperFunctions {
	static void SetLocation(Transform& trans, const DirectX::SimpleMath::Vector3& location) {
		trans.Location = location;
	}

	static void SetRotation(Transform& trans, const DirectX::SimpleMath::Vector3& rotation) {
		trans.Rotation = rotation;
	}

	static void SetScale(Transform& trans, const DirectX::SimpleMath::Vector3& scale) {
		trans.Scale = scale;
	}

	extern void SetLocation(Actor* pAct, const DirectX::SimpleMath::Vector3& location);
	extern void SetRotation(Actor* pAct, const DirectX::SimpleMath::Vector3& rotation);
	extern void SetScale(Actor* pAct, const DirectX::SimpleMath::Vector3& scale);

	extern void SetLocationY(Actor* pAct, float locationY);
	extern void SetRotationY(Actor* pAct, float angleY);

	extern const DirectX::SimpleMath::Vector3& GetLocation(Actor* pAct);
	extern const DirectX::SimpleMath::Vector3& GetRotation(Actor* pAct);
	extern const DirectX::SimpleMath::Vector3& GetScale(Actor* pAct);

	extern float GetRotationY(Actor* pAct);

	extern const DirectX::SimpleMath::Vector2& GetLocationFromSky(Actor* pAct);
	extern void SetLocationFromSky(Actor* pAct, DirectX::SimpleMath::Vector2 locationXZ);
}

namespace thf = TransformHelperFunctions;

/*
	externを使った理由
	static関数は、本来ファイルスコープ内で定義するのがルールとのこと。
	だが別ファイルに定義するときはexturnが必要らしいので使用した。
*/