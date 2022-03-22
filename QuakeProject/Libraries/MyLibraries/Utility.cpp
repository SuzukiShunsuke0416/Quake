#include "pch.h"
#include "Utility.h"


const BattleData BattleData::Default =
{
	0,
	0,
	0,
};

Int2 Int2::operator+(const Int2& a)
{
	return Int2(x + a.x, y + a.y);
}

Int2 Int2::operator-(const Int2& a)
{
	return Int2(x - a.x, y - a.y);
}

Int2 Int2::operator*=(const float& f)
{
	DirectX::SimpleMath::Vector2 hoz;
	hoz.x = (float)(x);
	hoz.y = (float)(y);
	hoz *= f;
	x = (int)(hoz.x);
	y = (int)(hoz.y);

	return *this;
}
