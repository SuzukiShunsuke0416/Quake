/**
 * @file InputManager.h
 * @brief 入力（マウスとキーボード）を管理するクラス
 * @author suzuki.shunsuke
 * @date 2022/01/30
 */
#pragma once
#include "SingletonFounder.h"

#include <Mouse.h>
#include <Keyboard.h>

 /**
 * @brief 入力（マウスとキーボード）を管理するクラス
 */
class InputManager :public SingletonFounder<InputManager> {
	friend SingletonFounder;

private:
	/**
	* @brief コンストラクタ
	*/
	InputManager();

private:
	/** マウス */
	std::unique_ptr<DirectX::Mouse> mpMouse;
	/** キーボード */
	std::unique_ptr<DirectX::Keyboard> mpKeyboard;

	/** マウストラッカー */
	DirectX::Mouse::ButtonStateTracker mMouseTracker;
	/** キーボードトラッカー */
	DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;
public:
	/**
	* @brief デストラクタ
	*/
	~InputManager() = default;

	/**
	* @brief 更新
	*/
	void Update();

	/**
	 * @brief マウストラッカー取得（const参照）
	 */
	const DirectX::Mouse::ButtonStateTracker& GetMouseTracker() {
		return mMouseTracker;
	}

	/**
	 * @brief キーボードトラッカー取得（const参照）
	 */
	const DirectX::Keyboard::KeyboardStateTracker& GetKeyboardTracker() {
		return mKeyboardTracker;
	}
};