/**
 * @file	StageTriangle.cpp
 * @brief	�I�[�i�[�̑����ɂ���n�ʏ��i�O�p�j
			���܂Ƃ߂��R���|�[�l���g
 * @author	suzuki.shunsuke
 * @date	2022/02/02
 */
#include "pch.h"
#include "StageTriangle.h"
#include "StagePoint.h"
#include "StageManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
StageTriangle::StageTriangle(Actor* pOwner)
	:Component(pOwner)
	,mpStagePoint()
	,mNormal()
	,mNormalRotationMatrix()
	,mAzimuth()
	,mElevationAngle()
{
	StageManager::GetInstance()->AddTriangleMember(this);
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
StageTriangle::~StageTriangle()
{
	StageManager::GetInstance()->RemoveTriangleMember(this);
}

//=====================================================
//		�X�V
//=====================================================
void StageTriangle::Update()
{
	const Vector3 triPos1 = thf::GetLocation(mpStagePoint[0]);
	const Vector3 triPos2 = thf::GetLocation(mpStagePoint[1]);
	const Vector3 triPos3 = thf::GetLocation(mpStagePoint[2]);

	// �@���x�N�g���ݒ�
	mNormal = (triPos3 - triPos1).Cross(triPos2 - triPos1);

	// ���ʊp
	mAzimuth = atan2f(mNormal.z, -mNormal.x);

	// �p
	mElevationAngle = atan2f(Vector2(mNormal.x, mNormal.z).Length(), mNormal.y);

	// ��]�s��ݒ�
	mNormalRotationMatrix = Matrix::Identity;
	mNormalRotationMatrix *= Matrix::CreateRotationZ(mElevationAngle);
	mNormalRotationMatrix *= Matrix::CreateRotationY(mAzimuth);
}
