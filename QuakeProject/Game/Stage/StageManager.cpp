/**
 * @file StageManager.cpp
 * @brief �X�e�[�W�����Ǘ�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/03
 */
#include "pch.h"
#include "StageManager.h"
#include "Game/DataBase/ExternalDatabase.h"
#include "Game/ElevationColor/ElevationColorManager.h"
#include "DeviceResources.h"
#include "Game/Collision/SimpleCheckFunctions.h"
#include "StageTriangleManager.h"
#include "Libraries/MyLibraries/PerlinNoise.h"
#include "Game/Camera/CameraManager.h"
#include "Game/DataBase/ExternalDatabase.h"
#include <numeric>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
StageManager::StageManager()
	:mpStagePoints()
	,mStageSise()
	,mPointDistance()
	,mMaxHeight()
	,mTopColor()
	,mGroundColor()
	,mpLineBatchDatas()
	,mCenterPosition()
	,mpStageTriangleManager()
	,mStageSidePosBox(0.0f,0.0f,0.0f,0.0f)
	,mpBright()
{
	mpLineBatchDatas = new DataForPrimitiveBatchRender<VertexPositionColorTexture>();
	mpStageTriangleManager = new StageTriangleManager();
	mpBright = std::make_unique<BrightChanger>();
	mpBright->ChangeConstBuffer({ 1.0f,0.25f,0.0008f,0.0f });

}

//=====================================================
//		�f�X�g���N�^
//=====================================================
StageManager::~StageManager()
{
	delete mpLineBatchDatas;
	mpLineBatchDatas = nullptr;

	delete mpStageTriangleManager;
	mpStageTriangleManager = nullptr;
}

//=====================================================
//		������
//=====================================================
void StageManager::Initialize()
{
	// �X�e�[�W�f�[�^�̃p�����[�^���O���t�@�C������ǂݍ���
	std::vector<std::string> data;	// �O���t�@�C���̏����i�[����z��
	int sizeX, sizeY;				// �X�e�[�W�̃T�C�Y����
	auto dataBase = ExternalDatabase::GetInstance();// �O���t�@�C���ǂݍ��݃}�l�[�W��
	dataBase->GetDataFromPath(dataBase->GetFilePath(eExPathType::StageData), data);
	// �O���t�@�C���̏��ԂƑ����āA�f�[�^��ϐ���Ɋi�[
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[0].c_str(), "%d", &sizeX);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[1].c_str(), "%d", &sizeY);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[2].c_str(), "%f", &mPointDistance);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[3].c_str(), "%f", &mMaxHeight);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[4].c_str(), "%f,%f,%f", &mTopColor.r, &mTopColor.g, &mTopColor.b);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[5].c_str(), "%f,%f,%f", &mGroundColor.r, &mGroundColor.g, &mGroundColor.b);


	{// �|�C���g�z��̗v�f��������\�ߐݒ�
		using namespace std;
		mpStagePoints = vector<vector<StagePoint*>>(sizeY, vector<StagePoint*>(sizeX));
	}

	// �X�e�[�W�|�C���g�̐���
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mpStagePoints[y][x] = new StagePoint();
		}
	}

	// �p�[�����m�C�Y������𐶐�
	PerlinNoise NoiseMaker;

	// �X�e�[�W�|�C���g�̏�����
	StagePointInitDesc desc;
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			// ���W
			desc.pos = Vector3(
				(float)(x)*mPointDistance,
				0.0f,
				(float)(y)*mPointDistance
			);

			// �v�f�ԍ�
			desc.element = { x,y };

			// �אړ_
			int nextX = std::min(x + 1, sizeX - 1);
			int nextY = std::min(y + 1, sizeY - 1);
			desc.nextRight = mpStagePoints[y][nextX];
			desc.nextBottom = mpStagePoints[nextY][x];
			desc.nextRightBottom = mpStagePoints[nextY][nextX];

			// �v���~�e�B�u�o�b�`
			desc.pLineBatchDatas = mpLineBatchDatas;

			// ������
			mpStagePoints[y][x]->Initialize(desc, NoiseMaker);
		}
	}

	// ����̃X�e�[�W�T�C�Y
	mStageSise = Int2(sizeX, sizeY);

	// �X�e�[�W�̒[���W
	mStageSidePosBox.left = 0.0f;
	mStageSidePosBox.top = 0.0f;
	mStageSidePosBox.right = (sizeX - 1) * mPointDistance;
	mStageSidePosBox.bottom = (sizeY - 1) * mPointDistance;

	// �����l�ݒ�
	mCenterPosition.x = mStageSidePosBox.right / 2.0f;
	mCenterPosition.z = mStageSidePosBox.bottom / 2.0f;

	this->CalculateStageTriangle();
}

//=====================================================
//		�X�V
//=====================================================
void StageManager::Update()
{
	// �|�C���g�B���X�V
	for (const auto& linePoints : mpStagePoints) {
		for (const auto& point : linePoints) {
			point->Update();
		}
	}

	// StageTriangle�����̒l�ݒ�
	this->CalculateStageTriangle();

	//this->SetPolygonDatas();
}

//=====================================================
//		�`��
//=====================================================
void StageManager::Render()
{
	auto& view = CameraManager::GetInstance()->GetViewMatrix();
	auto& proj = CameraManager::GetInstance()->GetProjectionMatrix();
	mpLineBatchDatas->BasicEffectApply(view, proj);


	//TODO �ʗp�V�F�[�_��K�p

	for (const auto& line : mpStagePoints) {
		for (const auto& point : line) {
			point->Render();
		}
	}


	/*mpLineBatchDatas->primitiveBatch->Begin();

	//�ʈꊇ�`��
	mpLineBatchDatas->primitiveBatch->DrawIndexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		&mIndices[0],
		mIndices.size(),
		&mVertices[0],
		mVertices.size()
	);

	mpLineBatchDatas->primitiveBatch->End();*/

	mpLineBatchDatas->BasicEffectApply(view, proj);

	mpBright->Begin();// �����J�n

	for (const auto& line : mpStagePoints) {
		for (const auto& point : line) {
			point->LineRender();
		}
	}

	mpBright->End();// �����I��
}

//=====================================================
//		�I������
//=====================================================
void StageManager::Finalize()
{
	// �X�e�[�W�|�C���g���폜
	for (unsigned y = 0; y < mpStagePoints.size(); y++) {
		for (unsigned x = 0; x < mpStagePoints[y].size(); x++) {
			delete mpStagePoints[y][x];
			mpStagePoints[y][x] = nullptr;
		}
	}
}

//=====================================================
//		StageTriangle�����o�̒ǉ�
//=====================================================
void StageManager::AddTriangleMember(StageTriangle* triangle)
{
	mpStageTriangleManager->AddMember(triangle);
}

//=====================================================
//		StageTriangle�����o�̏��O
//=====================================================
void StageManager::RemoveTriangleMember(StageTriangle* triangle)
{
	mpStageTriangleManager->RemoveMember(triangle);
}

//=====================================================
//		StageTriangle�����̒l�ݒ�
//=====================================================
void StageManager::CalculateStageTriangle()
{
	// �I�[�i�[�ɋ߂��_���R�I�ё����̂R�p�`��ݒ肷��
	
	int left, right, top, bottom;	// �����S�_�̃X�e�[�W�|�C���g�̗v�f�ԍ����i�[
	Vector3 ownerPos;				// �I�[�i�[�̍��W�i�R�����j
	Vector2 ownerPosV2;				// �I�[�i�[�̍��W�iXZ�ʁj
	StagePoint* nearPoints[4];		// �I�[�i�[�t�߂̃X�e�[�W�|�C���g�S�_
	StagePoint** points = nullptr;	// �X�e�[�W�g���C�A���O�������R�_�̃|�C���g�z��̐擪�A�h���X
	// ���݂���S�Ă�StageTriangle
	const auto& Triangles = mpStageTriangleManager->GetStageTriangles();

	// �S�����g���C�A���O���̐ݒ���s��
	for (auto triangle : Triangles) {
		// �I�[�i�[�̍��W�i�R�����AXZ�ʁj�̎擾
		ownerPos = thf::GetLocation(triangle->GetOwner());
		ownerPosV2 = Vector2(ownerPos.x, ownerPos.z);
		// XZ�ʍ��W����X�e�[�W�|�C���g�̗v�f�ԍ����v�Z����
		left = (int)(ownerPosV2.x / mPointDistance);
		top = (int)(ownerPosV2.y / mPointDistance);
		right = left + 1;
		bottom = top + 1;

		// �v�f�ԍ����X�e�[�W�̃T�C�Y�𒴂��Ă����珈�����΂��B
		BoxParam<int> inBox = BoxParam<int>(left, right, top, bottom);
		BoxParam<int> outBox = BoxParam<int>(0, mStageSise.x - 1, 0, mStageSise.y - 1);
		if (SimpleCheckFunctions::IsBoxInBox(outBox, inBox) == false)continue;

		// �t�߂S�̃X�e�[�W�|�C���g�𔲂��o��
		nearPoints[0] = mpStagePoints[top][left];
		nearPoints[1] = mpStagePoints[top][right];
		nearPoints[2] = mpStagePoints[bottom][right];
		nearPoints[3] = mpStagePoints[bottom][left];

		// �l�p�`�̑Ίp�����`������Q�_���������c��̂Q�_�ŋ߂������g�p���R�p�`�𐶐�����B
		if ((thf::GetLocationFromSky(nearPoints[1]) - ownerPosV2).Length() >
			((thf::GetLocationFromSky(nearPoints[3]) - ownerPosV2).Length())) 
		{
			nearPoints[1] = nearPoints[2];
			nearPoints[2] = nearPoints[3];
		}

		// �O�p�`�̂R�_�̃X�e�[�W�|�C���g������������
		points = triangle->GetStagePoints();
		for (int i = 0; i < StageTriangle::POINT_SIZE; i++) {
			points[i] = nearPoints[i];
		}

		// ���̑�StageTriangle�̍X�V�������s��
		triangle->Update();
	}
}

//=====================================================
//		�ʕ`�掞�Ɏg���l�����̐ݒ�
//=====================================================
void StageManager::SetPolygonDatas()
{
	// �X�e�[�W�|�C���g�̑���
	int serialLength = mStageSise.x * mStageSise.y;

	//============= ���_��� =============

	// ��U�z�����Ɂi�L���p�͐�Ɋm�ہj
	mVertices.clear();
	mVertices.reserve(serialLength);

	// ���F����ݒ�
	Vector4 subtractiveColorRat;
	{
		float r = 0.5f;
		subtractiveColorRat = Vector4(r, r, r, 1.0f);
	}
	
	// ���_����ݒ�i�P���ɔz��̏��ԂŁj
	for (const auto& linePoints : mpStagePoints) {
		for (const auto& point : linePoints) {
			auto vertex = VertexPositionColorTexture(
				point->GetTransform().Location,
				point->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
				//Colors::Red,
				Vector2::Zero
			);

			mVertices.push_back(vertex);
		}
	}

	//============= ���ԏ�� =============
	
	// ��U�z������
	mIndices.clear();

	// ���ݒǉ��ݒ肵�Ă���v�f�ԍ�
	Int2 indexI2 = Int2(0,-1);
	
	// ���_���ɃA�N�Z�X����ۂɎg�p�����C���f�b�N�X�̒l����Ȃ��̔z��Ƃ��čl���A�ݒ肷��ۂɎg�p����B
	int* serialNums = new int[serialLength + 1];
	std::iota(serialNums, serialNums + serialLength + 1, 0);//�����A�Ԑݒ�֐�


	//���ݍ��E�̂ǂ���ɐi��ł��邩
	const unsigned moveLeft =	(1 << 0);
	const unsigned moveRight =	(1 << 1);
	const unsigned moveUp =		(1 << 2);
	const unsigned moveDown =	(1 << 3);

	unsigned moveWay = moveUp;

	//moveWay�̒l�ɉ�����indexI2�̒l��ݒ肷��֐�
	auto SetIndexWithWay = [&]() {
		if (moveWay & moveLeft)	{ indexI2.x--; }
		if (moveWay & moveRight){ indexI2.x++; }
		if (moveWay & moveUp)	{ indexI2.y--; }
		if (moveWay & moveDown)	{ indexI2.y++; }
	};

	//���݂̈ړ����������Ɏ��̈ړ�������ݒ肷��֐�
	//����̎��͉E���A�E���̎��͏�
	//�����̎��͍���A����̎��͉�
	auto SetWaywithWay = [&]() {
		if		(moveWay == (moveUp))
			{ moveWay = (moveRight | moveDown); }

		else if (moveWay == (moveRight | moveDown))
			{ moveWay = moveUp; }
		
		else if (moveWay == (moveDown))
			{ moveWay = (moveLeft | moveUp); }
		
		else if (moveWay == (moveLeft | moveUp))
			{ moveWay = moveDown; }
	};

	//indexI2�̒l�ɉ����ĘA�Ԓl��Ԃ��֐�
	auto SetInediceWithSerial = [&]() {
		if (indexI2.y >= mStageSise.y) return true;

		if (indexI2.y < 0)return false;

		mIndices.push_back(serialNums[indexI2.x + indexI2.y * mStageSise.x]);
		return false;
	};

	//�S�Ă̒l��ݒ肷��܂�
	while (true) 
	{
		
		// ���[�ɗ��Ă���ꍇ
		if (indexI2.x == 0) {
			SetInediceWithSerial();

			//���݂̉��̒l��ǉ�
			indexI2.y++;
			SetInediceWithSerial();

			//�X�ɂ����P���̒l��ǉ�
			indexI2.y++;
			bool isOver = SetInediceWithSerial();
			if (isOver)break;

			//�E�Ɉ���炵�Ă���
			indexI2.y--;
			SetInediceWithSerial();

			//�����u��v�ɂȂ�悤�ɁA�ꎞ�I�Ɂu�E���v�Ɛݒ�
			moveWay = (moveRight | moveDown);

			indexI2.x++;
			indexI2.y++;
		}
		// �E�[�ɗ��Ă���ꍇ
		else if (indexI2.x == mStageSise.x - 1) {
			//�܂����݂̏�̒l��ǉ�
			//indexI2.y--;
			SetInediceWithSerial();

			//���̌セ�̉��i�������ꏊ�j�̒l��ǉ�
			indexI2.y--;
			SetInediceWithSerial();

			//����ɂ��̉��̒l��ǉ�
			indexI2.y++;
			indexI2.y++;
			bool isOver = SetInediceWithSerial();
			if (isOver)break;

			// ����ɂ��炵�Ă���
			indexI2.x--;
			indexI2.y--;

			//����
			moveWay = (moveLeft | moveUp);
		}

		//���݂̒l��ǉ�
		SetInediceWithSerial();

		//�ړ������t���O�̐ݒ�
		SetWaywithWay();
		//���̏ꏊ�ւ��炷
		SetIndexWithWay();

	}

	delete[] serialNums;
}
