#include "pch.h"
#include "ControlCamera.h"
#include "Game/UtilityManager/InputManager.h"
#include "Libraries/MyLibraries/Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �����_�̈ʒu�i�I�[�i�[�{�����ォ�j
const float ControlCamera::ADD_VIEW_HEIGHT = 10.0f;

// �I�[�i�[�Ƃ̋���
const Vector3 ControlCamera::DISTANCE = Vector3(0.0f, 24.0f, 10.0f);

//=====================================================
//		�R���X�g���N�^
//=====================================================
ControlCamera::ControlCamera(Actor* pOwner)
	:MyCamera(pOwner)
	,mMouseCursorPosLog()
	,mDistanceRat(1.0f)
{
	const auto& mouseTrack = InputManager::GetInstance()->GetMouseTracker();
	mMouseCursorPosLog.x = mouseTrack.GetLastState().x;
	mMouseCursorPosLog.y = mouseTrack.GetLastState().y;
}

//=====================================================
//		�f�X�g���N�^
//=====================================================
ControlCamera::~ControlCamera()
{
}

//=====================================================
//		�X�V
//=====================================================
void ControlCamera::Update()
{
	// �I�[�i�[�ƃJ�����̋��������v�Z
	Vector3 distance;
	this->CalculateDistance(distance);

	// �I�[�i�[�̍��W
	Vector3 ownerPos = thf::GetLocation(mpOwnerActor);

	// �J�������W�ύX
	this->SetEyePosition(ownerPos + distance);

	// �����_���W�ύX
	this->SetTargetPosition(ownerPos + Vector3(0.0f,ADD_VIEW_HEIGHT * mDistanceRat,0.0f));

	// �r���[�E�v���W�F�N�V�����s���ݒ�
	this->CalculateViewMatrix();

	//TODO �I�[�i�[����̑��x������
	const auto& keyboard = InputManager::GetInstance()->GetKeyboardTracker().GetLastState();
	if (keyboard.W || keyboard.S || keyboard.A || keyboard.D) {
		myf::RubberComplementSlow(mDistanceRat, 1.0f);
	}
	else {
		myf::RubberComplementSlow(mDistanceRat,0.5f);
	}
}

//=====================================================
//		�}�E�X����ɍ��킹�ċ�����ς���
//=====================================================
void ControlCamera::CalculateDistance(Vector3& distance)
{
	// �g���b�J�[���擾
	const auto& mouseTrack = InputManager::GetInstance()->GetMouseTracker();

	// ���݂̃}�E�X�J�[�\���̈ʒu���擾
	Int2 cursorPos = Int2(mouseTrack.GetLastState().x, mouseTrack.GetLastState().y);

	// ���V�t�g�L�[��������Ă�����A������
	if (mouseTrack.GetLastState().rightButton == true) {
		// �J�[�\���ړ��ʂ�ݒ�
		mMoveVol.x -= (float)(cursorPos.x - mMouseCursorPosLog.x);
		mMoveVol.y -= (float)(cursorPos.y - mMouseCursorPosLog.y);

		// �㉺����̌��E�l��ݒ�
		myf::ClampRef(mMoveVol.y, 1200.0f, 0.0f);
	}

	// �J�[�\���ړ��ʂ����Ɋp�x�����
	Matrix eyeMat = Matrix::CreateTranslation(DISTANCE * mDistanceRat);
	eyeMat *= Matrix::CreateRotationX(mMoveVol.y * 0.001f);
	eyeMat *= Matrix::CreateRotationY(mMoveVol.x * 0.002f);

	DebugLogManager::GetInstance()->EntryLog(mMoveVol.y, L"MouseY");

	// �I�[�i�[�ƃJ�����̈ʒu����ݒ�
	distance = eyeMat.Translation();

	// �J�[�\�����O�l���X�V
	mMouseCursorPosLog = cursorPos;
}
