#include "pch.h"
#include "PlayerVisual.h"
#include "Player.h"
#include "DeviceResources.h"
#include <Effects.h>
#include "Game/Stage/StageManager.h"
#include "Game/Stage/StageTriangleManager.h"
#include "Game/Stage/StagePoint.h"
#include "Game/UtilityManager/InputManager.h"
#include "Game/Camera/CameraManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
PlayerVisual::PlayerVisual(Player* pOwner)
	:CharacterVisual(pOwner)
	,mTankModel()
	,mCommonState()
	,mNormal()
	,mAzimuth(0.0f)
	,mElevationAngle(0.0f)
	,mpCaterLeft()
	,mpCaterRight()
{
	this->CreateModel();

	Cater_Init_Desc cid;
	cid.pOwner = this;
	cid.sideType = eCaterSideType::Left;
	mpCaterLeft = std::make_unique<CaterpillarShaft>(cid);
	cid.sideType = eCaterSideType::Right;
	mpCaterRight = std::make_unique<CaterpillarShaft>(cid);
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
PlayerVisual::~PlayerVisual()
{
}

//=====================================================
//		�X�V
//=====================================================
void PlayerVisual::Update()
{
	this->CalculateWorldMatrixPersonal();

	// �L���^�s���̍X�V
	mpCaterLeft->Update();
	mpCaterRight->Update();
}

//=====================================================
//		�`��
//=====================================================
void PlayerVisual::Render()
{
	auto context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	Matrix view = CameraManager::GetInstance()->GetViewMatrix();
	Matrix proj = CameraManager::GetInstance()->GetProjectionMatrix();

	for (int i = 0; i < PARTS_NUM; i++) {
		mTankModel[i].model->Draw(
			context,
			*mCommonState,
			mTankModel[i].world,
			view,
			proj
		);
	}

	// �L���^�s���̕`��
	mpCaterLeft->Render();
	mpCaterRight->Render();
}

//=====================================================
//		���f���B�̃��[���h�s��v�Z
//=====================================================
void PlayerVisual::CalculateWorldMatrixPersonal()
{
	StageTriangle* triangle = StageManager::GetInstance()
		->GetStageTriangleManager()
		->GetComponentFromActor(mpOwner);

	Vector3 way = mpOwner->GetVelocity();
	float moveWayAngle = atan2f(-way.x, -way.z);

	const Vector3 normal = triangle->GetNormal();
	float azimuth = triangle->GetAzimuth();
	float elevation = triangle->GetElevationAngle();

	//myf::RubberComplementQuick(mAzimuth, azimuth);
	myf::RubberComplementAngleAdjustmentCustom(mAzimuth,azimuth);
	myf::RubberComplementQuick(mElevationAngle, elevation);
	myf::RubberComplementQuick(mNormal, normal);

	DebugLogManager::GetInstance()->EntryLog(mNormal, L"normal");
	DebugLogManager::GetInstance()->EntryLog(mAzimuth, L"azimuth");
	DebugLogManager::GetInstance()->EntryLog(mElevationAngle, L"elevation");

	Matrix rotmatNoAdjustment = Matrix::Identity;
	Matrix rotmatmBase = Matrix::Identity;
	Matrix rotmatmRide = Matrix::Identity;
	Matrix rotmatmCannon = Matrix::Identity;
	MyCamera* playerCamera = CameraManager::GetInstance()->GetComponentFromActor(mpOwner);
	Vector3 playerPos = thf::GetLocation(mpOwner);
	Vector3 cameraWay = playerPos - playerCamera->GetEyePosition();
	
	{// �@����������Z�o������]�s��i���ʊp�����Ȃ��j
		rotmatNoAdjustment *= Matrix::CreateRotationZ(mElevationAngle);
		rotmatNoAdjustment *= Matrix::CreateRotationY(mAzimuth);
	}
	{// �i�s�����Ɍ����Ă�����
		Quaternion q = Quaternion::CreateFromAxisAngle(mNormal, -mAzimuth + moveWayAngle + XM_PIDIV2);
		rotmatmBase = Matrix::CreateFromQuaternion(q);
	}
	{// �J�����̕����Ɍ����Ă�����
		Quaternion q = Quaternion::CreateFromAxisAngle(mNormal, -mAzimuth + atan2f(-cameraWay.x, -cameraWay.z) + XM_PIDIV2);
		rotmatmRide = Matrix::CreateFromQuaternion(q);
	}
	{// ����(�C���p)
		Vector3 cross = (cameraWay*Vector3(1,0,1)).Cross(normal);
		cross.Normalize();
		cross = normal.Cross(cross);
		cross.Normalize();
		
		rotmatmCannon = Matrix::CreateRotationZ(atan2f(cross.y, Vector2(-cross.x, -cross.z).Length()) * -1);
	}
	

	mWorld = Matrix::Identity;

	// ���ʐݒ�
	for (int i = 0; i < PARTS_NUM; i++) {
		mTankModel[i].world = mWorld;
		mTankModel[i].world *= Matrix::CreateTranslation(mTankModel[i].ownDis);
		mTankModel[i].world *= Matrix::CreateScale(thf::GetScale(mpOwner) * 0.3f);
	}

	// �������炻�ꂼ���]������

	{// �x�[�X
		mTankModel[eBase].world *= rotmatNoAdjustment;
		mTankModel[eBase].world *= rotmatmBase;
	}
	{// ���C�h
		mTankModel[eRide].world *= rotmatNoAdjustment;
		mTankModel[eRide].world *= rotmatmRide;
	}
	{// �L���m��
		mTankModel[eCannon].world *= rotmatmCannon;
		mTankModel[eCannon].world *= mTankModel[eRide].world;
	}


	// �Ō�ɑS�����ړ�������
	for (int i = 0; i < PARTS_NUM; i++) {
		mTankModel[i].world *= Matrix::CreateTranslation(playerPos);
	}

	mWorld = mTankModel[eBase].world;
}

//=====================================================
//		���f������
//=====================================================
void PlayerVisual::CreateModel()
{
	// �O���f�[�^�ǂݍ��݁iplayer�o�R�j
	const ExternalPlayerData& externalDatas = mpOwner->GetExternalData();
	
	// �G�t�F�N�g�t�@�N�g���[�𐶐�
	auto device = DX::DeviceResources::GetInstance()->GetD3DDevice();
	std::unique_ptr<DirectX::EffectFactory> factory 
		= std::make_unique<DirectX::EffectFactory>(device);

	// �f�B���N�g����ݒ�
	factory->SetDirectory(L"Resources/Models");

	// ���f���𐶐�
	mTankModel[eBase].model = DirectX::Model::CreateFromCMO(
		device,
		externalDatas.baseModelPath,
		*factory
	);
	mTankModel[eRide].model = DirectX::Model::CreateFromCMO(
		device,
		externalDatas.rideModelPath,
		*factory
	);
	mTankModel[eCannon].model = DirectX::Model::CreateFromCMO(
		device,
		externalDatas.cannonModelPath,
		*factory
	);

	mTankModel[eBase].ownDis = Vector3(0.0f, -0.45f, 0.0f);
	mTankModel[eRide].ownDis = Vector3(0.0f, 1.0f, 0.0f);
	mTankModel[eCannon].ownDis = Vector3(1.0f, -0.45f, 0.0f);

	// �R�����X�e�[�g�̐���
	mCommonState = std::make_unique<DirectX::CommonStates>(device);
}
