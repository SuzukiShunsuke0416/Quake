/**
 * @file ControlCamera.h
 * @brief OwnerActorを中心に、マウス操作などで動くカメラクラス
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "MyCamera.h"
#include "CameraManager.h"
#include "Libraries/MyLibraries/Utility.h"

struct ControlCameraMaker;

/**
* @brief OwnerActorを中心に、マウス操作などで動くカメラ
*/
class ControlCamera final :public MyCamera {
	friend ControlCameraMaker;
public:
	/** 注視点の位置（オーナー＋何ｍ上か） */
	static const float ADD_VIEW_HEIGHT;
	/** オーナーとの距離 */
	static const DirectX::SimpleMath::Vector3 DISTANCE;

private:
	/** マウスのカーソル座標ログ */
	Int2 mMouseCursorPosLog;

	/** カメラの移動量 */
	DirectX::SimpleMath::Vector2 mMoveVol;

	/** カメラの距離割合 */
	float mDistanceRat;
private:
	/**
	* @brief コンストラクタ
	* @note (ControlCameraMakerのみ生成可能)
	*/
	ControlCamera(Actor* pOwner);

public:

	/**
	* @brief デストラクタ
	*/
	~ControlCamera();

public:
	/**
	* @brief 更新
	*/
	void Update() override;

private:

	/**
	* @brief マウス操作に合わせて距離を変える
	*/
	void CalculateDistance(DirectX::SimpleMath::Vector3& distance);
};

/**
* @brief ControlCameraを生成する
* @note CameraManagerのみ使用可能
*/
struct ControlCameraMaker {
	friend CameraManager;
	// (CameraManagerのみアクセス可能)
private:
	/**
	 * @brief	コントロールカメラを生成
	 * @param[in] pOwner	オーナーポインタ
	 * @return ControlCamera* ｺﾝﾄﾛｰﾙｶﾒﾗのポインタ
	 * @note （deleteは受取人が担当）
	 */
	static ControlCamera* SpawnNewControlCamera(Actor* pOwner) {
		return new ControlCamera(pOwner);
	}

};