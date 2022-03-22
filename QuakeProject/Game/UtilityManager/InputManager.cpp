/**
 * @file InputManager.cpp
 * @brief 入力（マウスとキーボード）を管理するクラス
 * @author suzuki.shunsuke
 * @date 2022/01/30
 */
#include "pch.h"
#include "InputManager.h"
#include "DeviceResources.h"

using namespace DirectX;

//=====================================================
//		コンストラクタ
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
//		更新
//=====================================================
void InputManager::Update()
{
	mMouseTracker.Update(mpMouse->GetState());
	mKeyboardTracker.Update(mpKeyboard->GetState());
}
