#include "pch.h"
#include "Transform.h"
#include "Game/Actor/Actor.h"

using namespace DirectX::SimpleMath;


const Transform Transform::Default =
{
	{Vector3(0.0f,0.0f,0.0f)},
	{Vector3(0.0f,0.0f,0.0f)},
	{Vector3(1.0f,1.0f,1.0f)}
};

void Transform::AddLocation(Vector3 addLocation)
{
	Location += addLocation;
}

void Transform::AddRotation(Vector3 addRot)
{
	Rotation += addRot;
}

const Matrix& Transform::ChangeMatrix()
{
	matrix = Matrix::Identity;
	matrix *= Matrix::CreateScale(Scale);
	matrix *= Matrix::CreateRotationX(Rotation.x);
	matrix *= Matrix::CreateRotationY(Rotation.y);
	matrix *= Matrix::CreateRotationZ(Rotation.z);
	matrix *= Matrix::CreateTranslation(Location);
	return matrix;
}

const Matrix& Transform::ChangeMatrix_IgnoreScale()
{
	matrix = Matrix::Identity;
	matrix *= Matrix::CreateRotationX(Rotation.x);
	matrix *= Matrix::CreateRotationY(Rotation.y);
	matrix *= Matrix::CreateRotationZ(Rotation.z);
	matrix *= Matrix::CreateTranslation(Location);
	return matrix;
}

const DirectX::SimpleMath::Vector2& Transform::ChangeLocationFromSky()
{
	LocationFromSky = Vector2(Location.x, Location.z);
	return LocationFromSky;
}

/********************************************************
	yTransformƒNƒ‰ƒX‚ðŽg‚¢‚â‚·‚­‚·‚éƒwƒ‹ƒp[ŠÖ”ŒQz
*********************************************************/
void TransformHelperFunctions::SetLocation(Actor* pAct, const Vector3& location)
{
	thf::SetLocation(pAct->GetTransformRef(), location);
}

void TransformHelperFunctions::SetRotation(Actor* pAct, const Vector3& rotation)
{
	thf::SetRotation(pAct->GetTransformRef(), rotation);
}

void TransformHelperFunctions::SetScale(Actor* pAct, const Vector3& scale)
{
	thf::SetScale(pAct->GetTransformRef(), scale);
}

void TransformHelperFunctions::SetLocationY(Actor* pAct, float locationY)
{
	Vector3 pos = thf::GetLocation(pAct);
	thf::SetLocation(pAct, Vector3(pos.x, locationY, pos.z));
}

void TransformHelperFunctions::SetRotationY(Actor* pAct, float angleY)
{
	Vector3 rot = thf::GetRotation(pAct);
	thf::SetRotation(pAct,Vector3(rot.x, angleY, rot.z));
}

const Vector3& TransformHelperFunctions::GetLocation(Actor* pAct)
{
	return pAct->GetTransform().Location;
}

const Vector3& TransformHelperFunctions::GetRotation(Actor* pAct)
{
	return pAct->GetTransform().Rotation;
}

const Vector3& TransformHelperFunctions::GetScale(Actor* pAct)
{
	return pAct->GetTransform().Scale;
}

float TransformHelperFunctions::GetRotationY(Actor* pAct)
{
	return thf::GetRotation(pAct).y;
}

const Vector2& TransformHelperFunctions::GetLocationFromSky(Actor* pAct)
{
	return pAct->GetTransformRef().ChangeLocationFromSky();
}

void TransformHelperFunctions::SetLocationFromSky(Actor* pAct, Vector2 locationXZ)
{
	pAct->GetTransformRef().Location.x = locationXZ.x;
	pAct->GetTransformRef().Location.z = locationXZ.y;
}

