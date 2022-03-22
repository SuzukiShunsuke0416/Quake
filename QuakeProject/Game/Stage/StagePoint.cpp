/**
 * @file StagePoint.cpp
 * @brief �X�e�[�W���\������P�̓_�i�A�N�^�[�j
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#include "pch.h"
#include "StagePoint.h"
#include "Game/ElevationColor/ElevationColorManager.h"
#include "StageManager.h"
#include "Game/Camera/CameraManager.h"

//#define FlatMode

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
StagePoint::StagePoint()
	:Actor()
	,mpNextRight(nullptr)
	,mpNextBottom(nullptr)
	,mpNextRightBottom(nullptr)
	,mpColor(nullptr)
	,mpPrimitiveBatchDatas(nullptr)
{
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
StagePoint::~StagePoint()
{
}

//=====================================================
//		������
//=====================================================
void StagePoint::Initialize(const StagePointInitDesc& desc, PerlinNoise& noiseMaker)
{
	mTransform.Location = desc.pos;
	mElement = desc.element;
	mpNextRight =		desc.nextRight;
	mpNextBottom =		desc.nextBottom;
	mpNextRightBottom =	desc.nextRightBottom;
	mpPrimitiveBatchDatas = desc.pLineBatchDatas;

	mpColor = new ElevationColor(this);

#ifdef FlatMode
	mTransform.Location.y = desc.element.x + desc.element.y;

#endif // FlatMode
#ifndef FlatMode
	mTransform.Location.y 
		= noiseMaker(thf::GetLocationFromSky(this) * 0.012f) 
		* StageManager::GetInstance()->GetMaxHeight();

#endif // !FlatMode

	mpNextPoints.insert(mpNextRight);
	mpNextPoints.insert(mpNextBottom);
	mpNextPoints.insert(mpNextRightBottom);
}

//=====================================================
//		�X�V
//=====================================================
void StagePoint::Update()
{
	/*DebugObjManager::GetInstance()->Entry(
		mTransform.Location,
		0.5f,
		mpColor->GetColor().V3()
	);*/
}

//=====================================================
//		�ʂ̕`��
//=====================================================
void StagePoint::Render()
{
	// ���F��
	Vector4 subtractiveColorRat = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	// ���_���W
	VertexPositionColorTexture ver[4] = {};

	ver[0] = VertexPositionColorTexture(// ���_�i����j
		mTransform.Location,
		mpColor->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);
	ver[1] = VertexPositionColorTexture(// �E�ׁi�E��j
		thf::GetLocation(mpNextRight),
		mpNextRight->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);
	ver[2] = VertexPositionColorTexture(// �^���i�����j
		thf::GetLocation(mpNextBottom),
		mpNextBottom->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);
	ver[3] = VertexPositionColorTexture(// �΂߉��i�E���j
		thf::GetLocation(mpNextRightBottom),
		mpNextRightBottom->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
		Vector2::Zero
	);

	// �v���~�e�B�u�o�b�`�`��J�n
	mpPrimitiveBatchDatas->primitiveBatch->Begin();
	//�l�p�`�`��
	mpPrimitiveBatchDatas->primitiveBatch->DrawQuad(ver[0], ver[1], ver[3], ver[2]);
	// �v���~�e�B�u�o�b�`�`��I��
	mpPrimitiveBatchDatas->primitiveBatch->End();
}

//=====================================================
//		���̕`��
//=====================================================
void StagePoint::LineRender()
{
	// �J��������view,proj�s����擾
	auto& view = CameraManager::GetInstance()->GetViewMatrix();
	auto& proj = CameraManager::GetInstance()->GetProjectionMatrix();

	//�@�x�[�V�b�N�G�t�F�N�g�K��
	mpPrimitiveBatchDatas->BasicEffectApply(view, proj);
	// �v���~�e�B�u�o�b�`�`��J�n
	mpPrimitiveBatchDatas->primitiveBatch->Begin();

	for (StagePoint* point : mpNextPoints) {
		// ����`��
		mpPrimitiveBatchDatas->primitiveBatch->DrawLine(
			VertexPositionColorTexture(mTransform.Location, mpColor->GetColor().V4(), Vector2::Zero),
			VertexPositionColorTexture(thf::GetLocation(point), point->GetElevationColor()->GetColor().V4(), Vector2::Zero)
		);
	}

	// �v���~�e�B�u�o�b�`�`��I��
	mpPrimitiveBatchDatas->primitiveBatch->End();
}
