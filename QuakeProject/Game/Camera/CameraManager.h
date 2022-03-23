/**
 * @file CameraManager.h
 * @brief ゲーム中に存在する全てのカメラを管理する
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Component/ComponentManager.h"
#include "MyCamera.h"
#include "Game/Camera/DebugCamera.h"

class ControlCamera;
class Camera;

/**
* @brief ゲーム中に存在する全てのカメラを管理する
*/
class CameraManager 
	: public SingletonFounder<CameraManager>
	, public ComponentManager<MyCamera>
{
	friend SingletonFounder;

private:
	/**
	* @brief コンストラクタ
	*/
	CameraManager();

	/**
	* @brief デストラクタ
	*/
	~CameraManager() = default;
private:
	/** 現在撮影を担当しているカメラ */
	Camera* mpActiveCamera;

	/** デバッグカメラ */
	std::unique_ptr<DebugCamera> mpDebugCamera;

	/** コントロールカメラ */
	ControlCamera* mpControlCamera;

public:


	/**
	* @brief 更新
	*/
	void Update() override;

	/**
	* @brief 描画
	*/
	void Render() override;

public:
	/**
	 * @brief	ControlCameraを生成、取得
	 * @param[in] pOwner	カメラを持つオーナーのポインタ
	 * @return SplitDataType カメラのポインタ
	 */
	ControlCamera* SpawnControlCamera(Actor* pOwner);

	/**
	 * @brief	アクティブカメラを変更する
	 * @param[in] id	変更するカメラのコンポーネントID
	 */
	inline void ChangeActiveCamera(int id) {
		mpActiveCamera = this->GetMemberFromID(id);
	}

public:
	/**
	 * @brief	使用中のカメラのview行列を返す
	 * @return view ビュー行列
	 */
	const DirectX::SimpleMath::Matrix& GetViewMatrix();

	/**
	 * @brief	使用中のカメラのprojection行列を返す
	 * @return projection プロジェクション行列
	 */
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix();

};