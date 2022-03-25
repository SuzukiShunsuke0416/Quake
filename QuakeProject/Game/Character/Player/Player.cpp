/**
 * @file Player.cpp
 * @brief �v���C���[�N���X
 * @author suzuki.shunsuke
 * @date 2022/02/06
 */
#include "pch.h"
#include "Player.h"
#include "Game/Stage/StageTriangle.h"
#include "Game/Stage/StagePoint.h"
#include "Game/Collision/CollisionManager.h"
#include "Game/Camera/CameraManager.h"
#include "Game/Camera/ControlCamera.h"
#include "Game/DataBase/ExternalDatabase.h"

using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
Player::Player()
	:CharacterActor()
	,mpStageTriangle()
	,mpCamera()
	,mpVisual()
	,mExternalData()
{
	thf::SetLocation(this, Vector3(10.0f, 100.0f, 10.0f));
	thf::SetScale(this, Vector3(3.0f));

	// �O���f�[�^��ǂݍ���
	this->LoadExternalData();

	// ����Triangle�𐶐�
	mpStageTriangle = new StageTriangle(this);

	// �����蔻��𐶐� TODO:Factory���Ȃɂ����
	new Collision(
		this,
		eCollisionType::Player,
		eCollisionType::Floor,
		[&](Collision* pCol) { this->Hit(pCol); }
	);

	// �J������ǉ�
	mpCamera = CameraManager::GetInstance()->SpawnControlCamera(this);

	// �����ڂ𐶐�
	mpVisual = std::make_unique<PlayerVisual>(this);

	// �S�X�e�[�g�𐶐�
	mpMoveState = std::make_unique<Player_MoveState>(this);
	mpAutoRunState = std::make_unique<Player_AutoRunState>(this);

	// �����X�e�[�g��ݒ�
	mpCurrentState = mpMoveState.get();
	mpCurrentState->Reset();
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
Player::~Player()
{
}

//=====================================================
//		�X�V
//=====================================================
void Player::Update()
{
	// ���X�e�[�g���X�V
	mpCurrentState->Update();

	Vector3 iro[] = { DirectX::Colors::Red ,DirectX::Colors::Green,DirectX::Colors::Blue };

	/*for (int i = 0; i < StageTriangle::POINT_SIZE; i++) {
		Vector3 pos = thf::GetLocation(mpStageTriangle->GetStagePoints(i));
		DebugObjManager::GetInstance()->Entry(pos,1.0f,iro[i]);
	}*/

	// �����ڂ̍X�V
	mpVisual->Update();

	DebugLogManager::GetInstance()->EntryLog(this->NowIsOnTheGround(),L"OnGround");

	CharacterActor::Update();

	DebugLogManager::GetInstance()->EntryLog(mVelocity, L"PlayerVel");
}

//=====================================================
//		�`��
//=====================================================
void Player::Render()
{
	// �����ڂ̕`��
	mpVisual->Render();
}

//=====================================================
//		�����蔻���̏���������֐�
//=====================================================
void Player::Hit(Collision* pCol)
{
	UNREFERENCED_PARAMETER(pCol);
}

//=====================================================
//		���n�C�x���g
//=====================================================
void Player::OnTouchGroundEvent()
{
	DebugLogManager::GetInstance()->EntryFriezeLog("OnPlayerLanding");
	SoundManager::GetInstance()->GetSound(eSoundType::Landing0)->Stop();
	SoundManager::GetInstance()->GetSound(eSoundType::Landing0)->Play();
}

//=====================================================
//		�O���f�[�^��ǂݍ���
//=====================================================
void Player::LoadExternalData()
{
	std::vector<std::wstring> data;	// �O���t�@�C���̏����i�[����z��

	ExternalDatabase::GetInstance()
		->GetDataFromType(eExPathType::PlayerData, data);

	wchar_t rowName[16];			// �v�f���i�g��Ȃ��j
	const int initPathLen = 64;
	const int modelNum = 3;
	mExternalData.allModelPath = new wchar_t*[modelNum];
	wchar_t** playerDataRef[modelNum] = 
	{
		&mExternalData.baseModelPath,
		&mExternalData.rideModelPath,
		&mExternalData.cannonModelPath,
	};


	for (int i = 0; i < modelNum; i++) 
	{
		mExternalData.allModelPath[i] = new wchar_t[initPathLen];

		swscanf_s(data[i].c_str(), L"%[^,],%ls",
			rowName, _countof(rowName),
			mExternalData.allModelPath[i], initPathLen
		);

		*(playerDataRef[i]) = mExternalData.allModelPath[i];
	}
}