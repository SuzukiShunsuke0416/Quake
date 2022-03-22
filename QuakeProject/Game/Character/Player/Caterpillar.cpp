/**
 * @file Caterpillar.cpp
 * @brief �L���^�s���p�[�c�N���X
 * @author suzuki.shunsuke
 * @date ???
 */

#include "pch.h"
#include "Caterpillar.h"
#include "Libraries/MyLibraries/Utility.h"
#include "CaterpillarShaft.h"

using namespace DirectX::SimpleMath;

/*==========================================================
	�y�R���X�g���N�^�z
==========================================================*/
CaterpillarParts::CaterpillarParts(CaterpillarShaft* pOwner)
	:mAngle(0.0f)
	,mPos()
	,mpOwner(pOwner)
{
}

/*==========================================================
	�y�f�X�g���N�^�z
==========================================================*/
CaterpillarParts::~CaterpillarParts()
{
}

/*==========================================================
	�y�X�V�z
==========================================================*/
void CaterpillarParts::Update(float plusAng)
{
	mAngle += plusAng;
	myf::Loop0to2PI(mAngle);

	const float halfSize = mpOwner->GetHalfSize();
	const float sideRat = mpOwner->GetSideRat();

	mPos.x = cosf(mAngle) * halfSize * sideRat;
	mPos.y = sinf(mAngle) * halfSize;

	const float size = halfSize * 0.5f;
	myf::ClampRef(mPos.y, size, -size);
}
