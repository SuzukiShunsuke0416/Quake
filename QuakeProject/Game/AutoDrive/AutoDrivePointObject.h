/**
 * @file AutoDrivePointObject.h
 * @brief 自動走行時に使うポイント
 * @author suzuki.shunsuke
 * @date 2022/03/25
 */
#pragma once

 /**
  * @brief 自動走行時に使うポイント
  */
class AutoDrivePointObject {
public:
	enum class Type {
		Start,

		Run,

		Goal,
	};

protected:

	/** 座標 */
	DirectX::SimpleMath::Vector3 mPos;

	/** ジオメトリックプリミティブ */
	std::unique_ptr<DirectX::GeometricPrimitive> mpGeometricPrimitive;
public:
	/**
	* @brief コンストラクタ
	* @param[in] pos	座標
	*/
	AutoDrivePointObject(
		DirectX::SimpleMath::Vector3 pos
	);

	/**
	* @brief デストラクタ
	*/
	virtual ~AutoDrivePointObject() = default;

	/**
	* @brief 更新
	*/
	virtual bool Update() = 0;

	/**
	* @brief 描画
	*/
	void Render();

public:

	/**
	* @brief 座標を取得
	*/
	inline const DirectX::SimpleMath::Vector3& GetPos() {
		return mPos;
	}

public:
	/**
	* @brief 対象物が自オブジェクトに触れたか
	*/
	bool IsHitObject();

public:
	/**
	* @brief オブジェクトポイントを生成する
	* @param[in] type	ポイントタイプ
	* @param[in] pos	座標
	*/
	static AutoDrivePointObject* CreatePointObj(
		Type type,
		DirectX::SimpleMath::Vector3 pos
	);
};

/**
  * @brief オートドライブポイント【スタート】
  */
class AutoDrivePoint_Start final :public AutoDrivePointObject {
private:

public:
	/**
	* @brief コンストラクタ
	* @param[in] pos	座標
	*/
	AutoDrivePoint_Start(DirectX::SimpleMath::Vector3 pos) 
		:AutoDrivePointObject(pos)
	{
	}

	/**
	* @brief デストラクタ
	*/
	~AutoDrivePoint_Start() = default;

	/**
	* @brief 更新
	*/
	bool Update() override;

};

/**
  * @brief オートドライブポイント【ラン】
  */
class AutoDrivePoint_Run :public AutoDrivePointObject {
private:

public:
	/**
	* @brief コンストラクタ
	* @param[in] pos	座標
	*/
	AutoDrivePoint_Run(DirectX::SimpleMath::Vector3 pos)
		:AutoDrivePointObject(pos)
	{
	}

	/**
	* @brief デストラクタ
	*/
	~AutoDrivePoint_Run() = default;

	/**
	* @brief 更新
	*/
	bool Update() override;

};

/**
  * @brief オートドライブポイント【ゴール】
  */
class AutoDrivePoint_Goal :public AutoDrivePointObject {
private:

public:
	/**
	* @brief コンストラクタ
	* @param[in] pos	座標
	*/
	AutoDrivePoint_Goal(DirectX::SimpleMath::Vector3 pos)
		:AutoDrivePointObject(pos)
	{
	}


	/**
	* @brief デストラクタ
	*/
	~AutoDrivePoint_Goal() = default;

	/**
	* @brief 更新
	*/
	bool Update() override;

};