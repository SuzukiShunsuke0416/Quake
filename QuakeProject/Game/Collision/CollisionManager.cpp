/**
 * @file CollisionManager.cpp
 * @brief 当たり判定を一括で管理する。
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "CollisionManager.h"
#include "Game/Stage/StageManager.h"
#include "Game/Stage/StageTriangleManager.h"
#include "Game/Character/CharacterManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
CollisionManager::CollisionManager()
	:ComponentManager()
	,mCheckMap()
	,mSingleFunctionMap()
{
	//===対系当たり判定、各判定関数===//


	//===単体系当たり判定、タイプ不一致時関数===//
	mSingleFunctionMap[eCollisionType::NONE] = [](CollisionManager* mana, Collision* pCol) 
	{
		UNREFERENCED_PARAMETER(mana); UNREFERENCED_PARAMETER(pCol);
	};

	//===単体系当たり判定、各判定関数===//
	mSingleFunctionMap[eCollisionType::Floor] = std::mem_fn(&CollisionManager::Character_vs_Floor);
}

//=====================================================
//		更新
//=====================================================
void CollisionManager::Update()
{
	const eCollisionType NOT_MATCH = eCollisionType::NONE;

	for (unsigned x = 0; x < mpMembers.size(); x++) 
	{
		// 「自分」のcollisionを抜き出す
		Collision* const member1 = mpMembers[x];
		// 「自分」のタイプを揃える
		const eCollisionType my1 = member1->GetMyType();
		const eCollisionType vs1 = member1->GetVsType();

		for (unsigned y = x + 1; y < mpMembers.size(); y++) 
		{
			// 「相手」のcollisionを抜き出す
			Collision* const member2 = mpMembers[y];
			// 「相手」のタイプを揃える
			const eCollisionType my2 = member2->GetMyType();
			const eCollisionType vs2 = member2->GetVsType();


			// 自分の当たる相手候補と、相手のタイプを見比べる
			if ((vs1 & my2) == NOT_MATCH) {
				continue;
			}

			// 自分と相手のタイプを元に呼ぶ関数を変える
			if (!mCheckMap[my1 | my2](this, member1, member2)) {
				continue;
			}

			// それぞれのオブジェクトに当たったと通告する
			member1->Hit(member2);
			member2->Hit(member1);
		}
		
		// 相手actor無し系判定
		if ((vs1 & eCollisionType::SINGLES) != NOT_MATCH) 
		{
			this->CheckSingles(member1, my1, vs1);
		}
	}
}

//=====================================================
//		 相手actor無し系判定をまとめる
//=====================================================
void CollisionManager::CheckSingles(
	Collision* const pCol,
	const eCollisionType myType,
	const eCollisionType vsType)
{
	UNREFERENCED_PARAMETER(myType);

	// 単身判定系を全走査
	for (eCollisionType type = eCollisionType::SINGLE_START;
		type <= eCollisionType::SINGLE_END; type = ctf::next(type))
	{
		// タイプに応じて関数を呼ぶ。
		mSingleFunctionMap[type & vsType](this, pCol);
	}

}

/********************************************************
* 
* ここから下が各当たり判定を担当する関数たち
* 
********************************************************/

//=====================================================
//		 キャラクター　対　地面
//=====================================================
void CollisionManager::Character_vs_Floor(Collision* pCol)
{
	CharacterActor* pCharacter = CharacterManager::GetInstance()
		->GetCharacter(pCol->GetOwner()->GetActorID());

	StageTriangle* triangle = StageManager::GetInstance()
		->GetStageTriangleManager()
		->GetComponentFromActor(pCharacter);

	StagePoint** points = triangle->GetStagePoints();

	const Vector3 triPos1 = thf::GetLocation(points[0]);
	const Vector3 triPos2 = thf::GetLocation(points[1]);
	const Vector3 triPos3 = thf::GetLocation(points[2]);
	const Vector3 charaPos = thf::GetLocation(pCharacter);
	const float charaSize = thf::GetScale(pCharacter).x * 0.5f;

	// 明らかに面から落ちていたら無理やり引き上げる
	if (charaPos.y < std::min({ triPos1.y,triPos2.y,triPos3.y })) {
		thf::SetLocationY(pCharacter, std::max({ triPos1.y,triPos2.y,triPos3.y }) + charaSize);
		DebugLogManager::GetInstance()->EntryFriezeLog("ForciblyUpFromColMana");
		return;
	}

	Vector3 normalVec = (triPos2 - triPos1).Cross(triPos3 - triPos1);
	normalVec.Normalize();

	const Vector3 charaTip = charaPos + normalVec * charaSize;

	const Vector3 toCharaWay = charaPos - triPos1;
	const Vector3 toTipWay = charaTip - triPos1;

	const float dot1 = normalVec.Dot(toCharaWay);
	const float dot2 = normalVec.Dot(toTipWay);

	// 線分が面をまたいでいなかったら無視(以降の押し出し処理をしない)
	if ((dot1 > 0) == (dot2 > 0)) {
		// 着地フラグをoffにする
		pCharacter->SetOnTheGroundFlag(false);
		return;
	}

	if (dot1 > 0) {
		thf::SetLocation(pCharacter, charaTip - (fabsf(dot1 * 2) + fabsf(dot2)) * normalVec);
	}
	else {
		thf::SetLocation(pCharacter, charaTip - (fabsf(dot1) + fabsf(dot2 * 2)) * normalVec);
	}

	// slipしない状態ならば、上移動のみ適応させる
	if (pCharacter->GetSlipFlag() == false) {
		Vector3 nowPos = thf::GetLocation(pCharacter);
		thf::SetLocation(pCharacter, Vector3(charaPos.x, nowPos.y, charaPos.z));
	}

	// もし今まで地面についていなかった場合、専用イベントを呼ぶ
	if (!pCharacter->NowIsOnTheGround()) {
		pCharacter->OnTouchGroundEvent();
	}

	// 着地フラグをonにする
	pCharacter->SetOnTheGroundFlag(true);
}
