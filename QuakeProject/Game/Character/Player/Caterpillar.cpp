/**
 * @file Caterpillar.cpp
 * @brief キャタピラパーツクラス
 * @author suzuki.shunsuke
 * @date ???
 */

#include "pch.h"
#include "Caterpillar.h"
#include "Libraries/MyLibraries/Utility.h"
#include "CaterpillarShaft.h"

using namespace DirectX::SimpleMath;

/*==========================================================
	【コンストラクタ】
==========================================================*/
CaterpillarParts::CaterpillarParts(CaterpillarShaft* pOwner)
	:mAngle(0.0f)
	,mPos()
	,mpOwner(pOwner)
{
}

/*==========================================================
	【デストラクタ】
==========================================================*/
CaterpillarParts::~CaterpillarParts()
{
}

/*==========================================================
	【更新】
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
