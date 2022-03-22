/**
 * @file InputManager.cpp
 * @brief ���́i�}�E�X�ƃL�[�{�[�h�j���Ǘ�����N���X
 * @author suzuki.shunsuke
 * @date 2022/01/30
 */
#include "pch.h"
#include "InputManager.h"
#include "DeviceResources.h"

using namespace DirectX;

//=====================================================
//		�R���X�g���N�^
//=====================================================
InputManager::InputManager() 
	:mpMouse()
	,mpKeyboard()
	,mMouseTracker()
	,mKeyboardTracker()
{
	mpMouse = std::make_unique<Mouse>();
	mpKeyboard = std::make_unique<Keyboard>();

	mpMouse->SetWindow(DX::DeviceResources::GetInstance()->GetHwnd());
}

//=====================================================
//		�X�V
//=====================================================
void InputManager::Update()
{
	mMouseTracker.Update(mpMouse->GetState());
	mKeyboardTracker.Update(mpKeyboard->GetState());
}
