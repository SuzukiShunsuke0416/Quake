/**
 * @file InputManager.h
 * @brief ���́i�}�E�X�ƃL�[�{�[�h�j���Ǘ�����N���X
 * @author suzuki.shunsuke
 * @date 2022/01/30
 */
#pragma once
#include "SingletonFounder.h"

#include <Mouse.h>
#include <Keyboard.h>

 /**
 * @brief ���́i�}�E�X�ƃL�[�{�[�h�j���Ǘ�����N���X
 */
class InputManager :public SingletonFounder<InputManager> {
	friend SingletonFounder;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	InputManager();

private:
	/** �}�E�X */
	std::unique_ptr<DirectX::Mouse> mpMouse;
	/** �L�[�{�[�h */
	std::unique_ptr<DirectX::Keyboard> mpKeyboard;

	/** �}�E�X�g���b�J�[ */
	DirectX::Mouse::ButtonStateTracker mMouseTracker;
	/** �L�[�{�[�h�g���b�J�[ */
	DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;
public:
	/**
	* @brief �f�X�g���N�^
	*/
	~InputManager() = default;

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	 * @brief �}�E�X�g���b�J�[�擾�iconst�Q�Ɓj
	 */
	const DirectX::Mouse::ButtonStateTracker& GetMouseTracker() {
		return mMouseTracker;
	}

	/**
	 * @brief �L�[�{�[�h�g���b�J�[�擾�iconst�Q�Ɓj
	 */
	const DirectX::Keyboard::KeyboardStateTracker& GetKeyboardTracker() {
		return mKeyboardTracker;
	}
};