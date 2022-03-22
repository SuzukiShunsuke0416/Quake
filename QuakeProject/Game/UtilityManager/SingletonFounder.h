/**
 * @file SingletonFounder.h
 * @brief �p����̃N���X���V���O���g���������邽�߂̃N���X
 * @author suzuki.shunsuke
 */
#pragma once

 /**
 * @brief �V���O���g���������邽�߂̃N���X
 */
template<typename T>
class SingletonFounder {
private:
	static T* msData;
public:
	static T* GetInstance() {
		if (msData == nullptr) {
			msData = new T();
		}
		return msData;
	}

	SingletonFounder(){
	}

	virtual ~SingletonFounder(){
	}

	void SingletonKill() {
		delete msData;
		msData = nullptr;
	}

	SingletonFounder(const SingletonFounder&) = delete;
	SingletonFounder& operator=(const SingletonFounder&) = delete;
	SingletonFounder(SingletonFounder&&) = delete;
	SingletonFounder& operator=(SingletonFounder&&) = delete;
};

template<typename T>
T* SingletonFounder<T>::msData = nullptr;

/*
	�y�p����N���X�ł�邱�Ɓz
	�@�R���X�g���N�^��private�Ɉڂ��B
	�A���̃N���X��friend�N���X�F�肳����B
	�B�y�ǋL�z�f�X�g���N�^��private�Ɉڂ��B
*/

/*
	�ύX 2/6(�y)
	���X���j�[�N�|�C���^�ō\�����Ă������A
	�܂��A�N�Z�X����\�肪����̂ɒ��r���[�ɍ폜�����
	���ۂ������������߁A�폜�^�C�~���O���������
	��߂�����ɕύX�����B
*/