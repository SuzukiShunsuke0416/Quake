/**
 * @file CharacterActor.cpp
 * @brief �L�����N�^�[�n�A�N�^�[
 * @author suzuki.shunsuke
 * @date 2022/02/04
 */
#include "pch.h"
#include "CharacterActor.h"
#include "Game/Stage/StageManager.h"

using namespace DirectX::SimpleMath;

 //=====================================================
 //		�X�V
 //=====================================================
void CharacterActor::Update()
{
	// 1f�O����̈ړ������v�Z
	mDistanceFrommBefore1f = mTransform.Location - mLocationLog;
	mLocationLog = mTransform.Location;

	// �d�͓K�p
	mVelocity.y -= mGravity;

	// ���W�ύX
	mTransform.Location += mVelocity;

	// ����
	mVelocity *= mSpeedDownRat;


	// �X�e�[�W����o�Ȃ��悤�ɂ���
	float size = thf::GetScale(this).x;
	Vector2 pos = thf::GetLocationFromSky(this);
	BoxParam<float> limitBox = StageManager::GetInstance()->GetSidePosBox();
	limitBox.Shrink(size, size);
	SimpleCheckFunctions::IsBoxInPointRef(limitBox, pos.x, pos.y);
	thf::SetLocationFromSky(this, pos);
}
