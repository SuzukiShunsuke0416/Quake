/**
 * @file CollisionManager.cpp
 * @brief �����蔻����ꊇ�ŊǗ�����B
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
//		�R���X�g���N�^
//=====================================================
CollisionManager::CollisionManager()
	:ComponentManager()
	,mCheckMap()
	,mSingleFunctionMap()
{
	//===�Όn�����蔻��A�e����֐�===//


	//===�P�̌n�����蔻��A�^�C�v�s��v���֐�===//
	mSingleFunctionMap[eCollisionType::NONE] = [](CollisionManager* mana, Collision* pCol) 
	{
		UNREFERENCED_PARAMETER(mana); UNREFERENCED_PARAMETER(pCol);
	};

	//===�P�̌n�����蔻��A�e����֐�===//
	mSingleFunctionMap[eCollisionType::Floor] = std::mem_fn(&CollisionManager::Character_vs_Floor);
}

//=====================================================
//		�X�V
//=====================================================
void CollisionManager::Update()
{
	const eCollisionType NOT_MATCH = eCollisionType::NONE;

	for (unsigned x = 0; x < mpMembers.size(); x++) 
	{
		// �u�����v��collision�𔲂��o��
		Collision* const member1 = mpMembers[x];
		// �u�����v�̃^�C�v�𑵂���
		const eCollisionType my1 = member1->GetMyType();
		const eCollisionType vs1 = member1->GetVsType();

		for (unsigned y = x + 1; y < mpMembers.size(); y++) 
		{
			// �u����v��collision�𔲂��o��
			Collision* const member2 = mpMembers[y];
			// �u����v�̃^�C�v�𑵂���
			const eCollisionType my2 = member2->GetMyType();
			const eCollisionType vs2 = member2->GetVsType();


			// �����̓����鑊����ƁA����̃^�C�v������ׂ�
			if ((vs1 & my2) == NOT_MATCH) {
				continue;
			}

			// �����Ƒ���̃^�C�v�����ɌĂԊ֐���ς���
			if (!mCheckMap[my1 | my2](this, member1, member2)) {
				continue;
			}

			// ���ꂼ��̃I�u�W�F�N�g�ɓ��������ƒʍ�����
			member1->Hit(member2);
			member2->Hit(member1);
		}
		
		// ����actor�����n����
		if ((vs1 & eCollisionType::SINGLES) != NOT_MATCH) 
		{
			this->CheckSingles(member1, my1, vs1);
		}
	}
}

//=====================================================
//		 ����actor�����n������܂Ƃ߂�
//=====================================================
void CollisionManager::CheckSingles(
	Collision* const pCol,
	const eCollisionType myType,
	const eCollisionType vsType)
{
	UNREFERENCED_PARAMETER(myType);

	// �P�g����n��S����
	for (eCollisionType type = eCollisionType::SINGLE_START;
		type <= eCollisionType::SINGLE_END; type = ctf::next(type))
	{
		// �^�C�v�ɉ����Ċ֐����ĂԁB
		mSingleFunctionMap[type & vsType](this, pCol);
	}

}

/********************************************************
* 
* �������牺���e�����蔻���S������֐�����
* 
********************************************************/

//=====================================================
//		 �L�����N�^�[�@�΁@�n��
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

	// ���炩�ɖʂ��痎���Ă����疳���������グ��
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

	// �������ʂ��܂����ł��Ȃ������疳��(�ȍ~�̉����o�����������Ȃ�)
	if ((dot1 > 0) == (dot2 > 0)) {
		// ���n�t���O��off�ɂ���
		pCharacter->SetOnTheGroundFlag(false);
		return;
	}

	if (dot1 > 0) {
		thf::SetLocation(pCharacter, charaTip - (fabsf(dot1 * 2) + fabsf(dot2)) * normalVec);
	}
	else {
		thf::SetLocation(pCharacter, charaTip - (fabsf(dot1) + fabsf(dot2 * 2)) * normalVec);
	}

	// slip���Ȃ���ԂȂ�΁A��ړ��̂ݓK��������
	if (pCharacter->GetSlipFlag() == false) {
		Vector3 nowPos = thf::GetLocation(pCharacter);
		thf::SetLocation(pCharacter, Vector3(charaPos.x, nowPos.y, charaPos.z));
	}

	// �������܂Œn�ʂɂ��Ă��Ȃ������ꍇ�A��p�C�x���g���Ă�
	if (!pCharacter->NowIsOnTheGround()) {
		pCharacter->OnTouchGroundEvent();
	}

	// ���n�t���O��on�ɂ���
	pCharacter->SetOnTheGroundFlag(true);
}