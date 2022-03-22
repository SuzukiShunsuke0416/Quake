/**
 * @file SingletonFounder.h
 * @brief 継承先のクラスをシングルトン化させるためのクラス
 * @author suzuki.shunsuke
 */
#pragma once

 /**
 * @brief シングルトン化させるためのクラス
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
	【継承先クラスでやること】
	①コンストラクタをprivateに移す。
	②このクラスをfriendクラス認定させる。
	③【追記】デストラクタもprivateに移す。
*/

/*
	変更 2/6(土)
	元々ユニークポインタで構成していたが、
	まだアクセスする予定があるのに中途半端に削除される
	事象が発生したため、削除タイミングをこちらで
	定める方式に変更した。
*/