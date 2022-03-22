/**
 * @file Player_MoveState.cpp
 * @brief �v���C���[�ړ��X�e�[�g
 * @author suzuki.shunsuke
 * @date 2022/02/12
 */
#include "pch.h"
#include "Player_MoveState.h"
#include "Game/Camera/CameraManager.h"
#include "Game/Camera/MyCamera.h"
#include "Game/UtilityManager/InputManager.h"
#include "../Player/Player.h"
#include "Libraries/MyLibraries/Utility.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
Player_MoveState::Player_MoveState(Player* pOwner)
	:CharacterStateBase(pOwner)
	,mpCamera()
	,mMoveWayAngle(0.0f)
	, mFrontWay()
{
	// �I�[�i�[�������Ă���J�������}�l�[�W���o�R�Ŏ󂯎��
	mpCamera = CameraManager::GetInstance()->GetComponentFromActor(mpOwner);
}

//=====================================================
//		���Z�b�g
//=====================================================
void Player_MoveState::Reset()
{
}

//=====================================================
//		�X�V
//=====================================================
void Player_MoveState::Update()
{
	// key�X�e�[�g���擾
	const auto& keytrack = InputManager::GetInstance()->GetKeyboardTracker();
	auto state = keytrack.GetLastState();

	// �W�����v
	if (keytrack.IsKeyPressed(Keyboard::Space) &&
		mpOwner->NowIsOnTheGround())
	{
		mpOwner->Jump();
	}

	// WASD�L�[�̓��͏�Ԃ�ݒ�
	const bool wasd[] = { state.W,	state.A, state.S, state.D };
	// �L�[�ɑΉ������p�x
	const int angles[] = { 0, 90, 180, 270 };
	// �����̃L�[�𓯎��������Ă��邩
	int multiPushCount = std::count(wasd, wasd + sizeof(wasd), true);

	// �����Ă��Ȃ�or�R�ȏ㉟���Ă���ꍇ�͖����B
	if (multiPushCount == 0 || multiPushCount >= 3) {
		mpOwner->SetSlipFlag(false);// ����Ȃ��悤��
		return;
	}
	else {
		mpOwner->SetSlipFlag(true);// ����悤��
	}

	// �����Ă���L�[�����Ɋp�x�����߂Ă���
	int inputAngle = 0;
	for (int i = 0; i < 4; i++) {
		inputAngle += angles[i] * (int)(wasd[i]);
	}

	// WD�̏ꍇ�i�E����́j�̏ꍇ��360�x�����Čv�Z�̒�������킹��
	inputAngle += ((int)(wasd[0]) * (int)(wasd[3]) * 360);
	// WS�̏ꍇ�i�㉺�����j�̏ꍇ�͒l���O�ɂ��邽�ߒ�������킹��
	inputAngle -= ((int)(wasd[0]) * (int)(wasd[2]) * 180);
	// �P�����͂̏ꍇ�͂��̂܂܂̒l�A�������͂̏ꍇ�͊Ԃ���邽�ߊ���B
	inputAngle /= multiPushCount;

	// �p�x�����炩�ɕύX����
	myf::RubberComplementAngleAdjustmentCustom(mMoveWayAngle, XMConvertToRadians((float)(inputAngle)));

	// �J�����̌����Ă���������擾����
	Vector3 frontWay = thf::GetLocation(mpOwner) - mpCamera->GetEyePosition();
	// �㉺�̒l�͖�������B
	frontWay.y = 0.0f;
	frontWay.Normalize();

	// �L�[���͂ɉ������p�x�l�ɉ����Đ��ʃx�N�g�������]������
	Matrix moveMat = Matrix::CreateTranslation(frontWay);
	moveMat *= Matrix::CreateRotationY(mMoveWayAngle);
	// �L�[���͂�K���������l�ɐݒ�
	frontWay = moveMat.Translation();
	// �I�[�i�[�̑��x�ϐ���ύX
	mpOwner->AddForce(frontWay * 0.5f);
}

//=====================================================
//		�I������
//=====================================================
void Player_MoveState::Finalize()
{
}

