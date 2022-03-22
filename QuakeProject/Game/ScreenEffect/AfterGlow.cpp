#include "pch.h"
#include "AfterGlow.h"
#include <Model.h>
#include "Game/Camera/CameraManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
AfterGlow::AfterGlow(float reductionRat, int glowSize)
{
	mReductionRat = reductionRat;
	mGlowSize = glowSize;

	mpBright = std::make_unique<BrightChanger>();
	mpBright->ChangeConstBuffer({ 1.0f,0.2f,0.0013f,0.0f });

	mUpdateLogFunction = std::mem_fn(&AfterGlow::PushModelMat);
}


//=====================================================
//		�c���K���J�n�i�޵��د�����è�ށj
//=====================================================
void AfterGlow::Adaptation(
	GeometricPrimitive* pGeoPri,
	Matrix ModelMat,
	XMVECTORF32 color
)
{
	// ���f���s�񃍃O�ǉ�
	mUpdateLogFunction(this, ModelMat);

	// �J�����֘A�̍s����擾
	const Matrix view = CameraManager::GetInstance()->GetViewMatrix();
	const Matrix proj = CameraManager::GetInstance()->GetProjectionMatrix();

	// �����`�揀���J�n
	mpBright->Begin();

	float size = 1.0f;
	for (Matrix world : mModelMatrixLogs) {
		Vector3 pos = world.Translation();
		world *= Matrix::CreateTranslation(-pos);
		world *= Matrix::CreateScale(size);
		world *= Matrix::CreateTranslation(pos);

		pGeoPri->Draw(world, view, proj, color);

		size *= mReductionRat;
	}

	// �����`��I��
	mpBright->End(true);

	DebugLogManager::GetInstance()->EntryLog(mModelMatrixLogs.size(), L"AfGlowSize");
}

//=====================================================
//		�c���K���J�n�i���f���j
//=====================================================
void AfterGlow::Adaptation(
	Model* pModel, 
	CommonStates* pCommonStates, 
	Matrix ModelMat)
{
	// ���f���s�񃍃O�ǉ�
	mUpdateLogFunction(this, ModelMat);

	// �����`�揀���J�n
	mpBright->Begin();



	// �����`��I��
	mpBright->End();
}

//=====================================================
//		���f���s��̃��O��ǉ�����
//=====================================================
void AfterGlow::PushModelMat(const Matrix& modelMat)
{
	// ���O�ǉ�
	mModelMatrixLogs.push_front(modelMat);

	// �����T�C�Y�I�[�o�[��������A����ȍ~�͍폜���s��
	if (mModelMatrixLogs.size() >= mGlowSize) {
		mUpdateLogFunction = std::mem_fn(&AfterGlow::PushPopModelMat);
	}
}

//=====================================================
//		���f���s��̃��O��ǉ��E�폜����
//=====================================================
void AfterGlow::PushPopModelMat(const DirectX::SimpleMath::Matrix& modelMat)
{
	// ���O�폜
	mModelMatrixLogs.pop_back();

	// ���O�ǉ�
	mModelMatrixLogs.push_front(modelMat);
}
