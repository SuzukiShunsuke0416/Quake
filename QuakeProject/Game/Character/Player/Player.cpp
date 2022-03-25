/**
 * @file Player.cpp
 * @brief プレイヤークラス
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
//		コンストラクタ
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

	// 外部データを読み込む
	this->LoadExternalData();

	// 足元Triangleを生成
	mpStageTriangle = new StageTriangle(this);

	// 当たり判定を生成 TODO:Factoryかなにか作る
	new Collision(
		this,
		eCollisionType::Player,
		eCollisionType::Floor,
		[&](Collision* pCol) { this->Hit(pCol); }
	);

	// カメラを追加
	mpCamera = CameraManager::GetInstance()->SpawnControlCamera(this);

	// 見た目を生成
	mpVisual = std::make_unique<PlayerVisual>(this);

	// 全ステートを生成
	mpMoveState = std::make_unique<Player_MoveState>(this);
	mpAutoRunState = std::make_unique<Player_AutoRunState>(this);

	// 活動ステートを設定
	mpCurrentState = mpMoveState.get();
	mpCurrentState->Reset();
}

//=====================================================
//		デストラクタ
//=====================================================
Player::~Player()
{
}

//=====================================================
//		更新
//=====================================================
void Player::Update()
{
	// 現ステートを更新
	mpCurrentState->Update();

	Vector3 iro[] = { DirectX::Colors::Red ,DirectX::Colors::Green,DirectX::Colors::Blue };

	/*for (int i = 0; i < StageTriangle::POINT_SIZE; i++) {
		Vector3 pos = thf::GetLocation(mpStageTriangle->GetStagePoints(i));
		DebugObjManager::GetInstance()->Entry(pos,1.0f,iro[i]);
	}*/

	// 見た目の更新
	mpVisual->Update();

	DebugLogManager::GetInstance()->EntryLog(this->NowIsOnTheGround(),L"OnGround");

	CharacterActor::Update();

	DebugLogManager::GetInstance()->EntryLog(mVelocity, L"PlayerVel");
}

//=====================================================
//		描画
//=====================================================
void Player::Render()
{
	// 見た目の描画
	mpVisual->Render();
}

//=====================================================
//		当たり判定後の処理をする関数
//=====================================================
void Player::Hit(Collision* pCol)
{
	UNREFERENCED_PARAMETER(pCol);
}

//=====================================================
//		着地イベント
//=====================================================
void Player::OnTouchGroundEvent()
{
	DebugLogManager::GetInstance()->EntryFriezeLog("OnPlayerLanding");
	SoundManager::GetInstance()->GetSound(eSoundType::Landing0)->Stop();
	SoundManager::GetInstance()->GetSound(eSoundType::Landing0)->Play();
}

//=====================================================
//		外部データを読み込む
//=====================================================
void Player::LoadExternalData()
{
	std::vector<std::wstring> data;	// 外部ファイルの情報を格納する配列

	ExternalDatabase::GetInstance()
		->GetDataFromType(eExPathType::PlayerData, data);

	wchar_t rowName[16];			// 要素名（使わない）
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