/**
 * @file CaterpillarShaft.cpp
 * @brief �L���^�s���N���X
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
	�y�R���X�g���N�^�z
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
	// �O���ǂݍ��݌n�f�[�^�̏�����
	this->InitExternalData();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	//�L���^�s���̊Ԋu��ݒ�
	const float CatRotDis = DirectX::XM_2PI / mCaterNum;

	//�L���^�s���𐶐��A������
	mCaterpillars.resize(mCaterNum);
	for (int i = 0; i < mCaterNum; i++) {
		mCaterpillars[i] = std::make_unique<CaterpillarParts>(this);
		mCaterpillars[i]->SetAngle(CatRotDis * i);
	}

	//�L���^�s���̖ʐ���
	mpCaterpillarPolygonData = std::make_unique<CaterpillarPolygonType>();
	mpCaterpillarPolygonData->SetRSState(mpCaterpillarPolygonData->commonState->CullNone());

	// ���点�S������
	mpBright = std::make_unique<BrightChanger>();
	mpBright->ChangeConstBuffer(Vector4(0.0f, 0.15f, 0.0013f, 0.0f));
}

/*==========================================================
	�y�f�X�g���N�^�z
==========================================================*/
CaterpillarShaft::~CaterpillarShaft()
{
}

/*==========================================================
	�y�X�V�z
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
	�y�`��z
==========================================================*/
void CaterpillarShaft::Render()
{
	//�ό`���̌�
	Matrix world = Matrix::CreateTranslation(Vector3(0.0f,0.0f, mDistanceFromTank * (float)mSideType));
	world *= mpOwner->GetPartsMatrix(PlayerVisual::eParts_Name::eBase);

	//�r���[�ƃv���W�F�N�V������ݒ�
	Matrix view = CameraManager::GetInstance()->GetViewMatrix();
	Matrix proj = CameraManager::GetInstance()->GetProjectionMatrix();

	// ��{�G�t�F�N�g�K�p
	mpCaterpillarPolygonData->BasicEffectApply(view, proj, world);

	// �L���^�s���̐F��n�ʐF�ɍ��킹��
	Vector4 color = StageManager::GetInstance()->GetStageTriangleManager()
		->GetComponentFromActor(mpOwner->GetOwnerPtr())->GetStagePoints(0)
		->GetElevationColor()->GetColor().V4();

	mpBright->Begin();//�����J�n

	for (int i = 0; i < mCaterpillars.size(); i++) {
		//���ԍ������Ɏ��̔ԍ������߂�
		int n = (i + 1 >= mCaterpillars.size()) ? 0 : i + 1;

		//���W���Z�b�g�i�L���^�s���B������W�����󂯎��j
		Vector3 pos = mCaterpillars[i]->GetPos();
		Vector3 pos2 = mCaterpillars[n]->GetPos();

		//�F���Z�b�g
		//Vector4 color = DirectX::Colors::Blue;
		Vector4 color2 = DirectX::Colors::Snow;

		//���_�����Z�b�g
		VertexPositionColor ver[] = {
			VertexPositionColor(pos - Vector3(0.0f,0.0f,mHalfWidthSize),color),
			VertexPositionColor(pos2 - Vector3(0.0f,0.0f,mHalfWidthSize),color2),
			VertexPositionColor(pos2 + Vector3(0.0f,0.0f,mHalfWidthSize),color2),
			VertexPositionColor(pos + Vector3(0.0f,0.0f,mHalfWidthSize),color)
		};

		//�`��
		mpCaterpillarPolygonData->primitiveBatch->Begin();
		mpCaterpillarPolygonData->primitiveBatch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
		mpCaterpillarPolygonData->primitiveBatch->End();
	}
	
	mpBright->End();// �����I��
}

/*==========================================================
	�y�O���ǂݍ��݌n�f�[�^�̏������z
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
