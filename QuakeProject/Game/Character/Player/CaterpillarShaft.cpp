/**
 * @file CaterpillarShaft.cpp
 * @brief キャタピラクラス
 * @author suzuki.shunsuke
 * @date ???
 */

#include "pch.h"
#include "CaterpillarShaft.h"
#include "DeviceResources.h"
#include "Game/Character/Player/PlayerVisual.h"
#include "Game/Camera/CameraManager.h"
#include "Game/Character/Player/Player.h"
#include "Game/Stage/StageTriangleManager.h"
#include "Game/Stage/StageManager.h"
#include "Game/DataBase/ExternalDatabase.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*==========================================================
	【コンストラクタ】
==========================================================*/
CaterpillarShaft::CaterpillarShaft(const Cater_Init_Desc& cid)
	:mpOwner(cid.pOwner)
	,mRotVel(-0.01f)
	,mCaterpillars()
	,mpCaterpillarPolygonData()
	,mSideType(cid.sideType)
	,mpBright()
	,mHalfSize()
	,mSideRat()
	,mCaterNum()
{
	// 外部読み込み系データの初期化
	this->InitExternalData();

	// D3Dデバイスとデバイスコンテキストの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	//キャタピラの間隔を設定
	const float CatRotDis = DirectX::XM_2PI / mCaterNum;

	//キャタピラを生成、初期化
	mCaterpillars.resize(mCaterNum);
	for (int i = 0; i < mCaterNum; i++) {
		mCaterpillars[i] = std::make_unique<CaterpillarParts>(this);
		mCaterpillars[i]->SetAngle(CatRotDis * i);
	}

	//キャタピラの面生成
	mpCaterpillarPolygonData = std::make_unique<CaterpillarPolygonType>();
	mpCaterpillarPolygonData->SetRSState(mpCaterpillarPolygonData->commonState->CullNone());

	// 光らせ担当生成
	mpBright = std::make_unique<BrightChanger>();
	mpBright->ChangeConstBuffer(Vector4(0.0f, 0.15f, 0.0013f, 0.0f));
}

/*==========================================================
	【デストラクタ】
==========================================================*/
CaterpillarShaft::~CaterpillarShaft()
{
}

/*==========================================================
	【更新】
==========================================================*/
void CaterpillarShaft::Update()
{
	Player* pPlayer = mpOwner->GetOwnerPtr();
	mRotVel = pPlayer->GetDistanceFrommBefore1fFromSky().Length() * -0.1f;

	for (auto& cat : mCaterpillars) {
		cat->Update(mRotVel);
	}
}

/*==========================================================
	【描画】
==========================================================*/
void CaterpillarShaft::Render()
{
	//変形情報の元
	Matrix world = Matrix::CreateTranslation(Vector3(0.0f,0.0f, mDistanceFromTank * (float)mSideType));
	world *= mpOwner->GetPartsMatrix(PlayerVisual::eParts_Name::eBase);

	//ビューとプロジェクションを設定
	Matrix view = CameraManager::GetInstance()->GetViewMatrix();
	Matrix proj = CameraManager::GetInstance()->GetProjectionMatrix();

	// 基本エフェクト適用
	mpCaterpillarPolygonData->BasicEffectApply(view, proj, world);

	// キャタピラの色を地面色に合わせる
	Vector4 color = StageManager::GetInstance()->GetStageTriangleManager()
		->GetComponentFromActor(mpOwner->GetOwnerPtr())->GetStagePoints(0)
		->GetElevationColor()->GetColor().V4();

	mpBright->Begin();//発光開始

	for (int i = 0; i < mCaterpillars.size(); i++) {
		//自番号を元に次の番号を決める
		int n = (i + 1 >= mCaterpillars.size()) ? 0 : i + 1;

		//座標をセット（キャタピラ達から座標だけ受け取る）
		Vector3 pos = mCaterpillars[i]->GetPos();
		Vector3 pos2 = mCaterpillars[n]->GetPos();

		//色をセット
		//Vector4 color = DirectX::Colors::Blue;
		Vector4 color2 = DirectX::Colors::Snow;

		//頂点情報をセット
		VertexPositionColor ver[] = {
			VertexPositionColor(pos - Vector3(0.0f,0.0f,mHalfWidthSize),color),
			VertexPositionColor(pos2 - Vector3(0.0f,0.0f,mHalfWidthSize),color2),
			VertexPositionColor(pos2 + Vector3(0.0f,0.0f,mHalfWidthSize),color2),
			VertexPositionColor(pos + Vector3(0.0f,0.0f,mHalfWidthSize),color)
		};

		//描画
		mpCaterpillarPolygonData->primitiveBatch->Begin();
		mpCaterpillarPolygonData->primitiveBatch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
		mpCaterpillarPolygonData->primitiveBatch->End();
	}
	
	mpBright->End();// 発光終了
}

/*==========================================================
	【外部読み込み系データの初期化】
==========================================================*/
void CaterpillarShaft::InitExternalData()
{
	std::vector<std::string> exData;
	ExternalDatabase::GetInstance()
		->GetDataFromType(eExPathType::CaterpillarData, exData);

	ExternalDatabase::SettingOwnParamExcludingElementName(
		exData[0].c_str(), "%f", &mHalfSize);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		exData[1].c_str(), "%f", &mSideRat);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		exData[2].c_str(), "%d", &mCaterNum);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		exData[3].c_str(), "%f", &mHalfWidthSize);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		exData[4].c_str(), "%f", &mDistanceFromTank);
}
