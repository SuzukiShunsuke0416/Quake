/**
 * @file ElevationColorManager.h
 * @brief ElevationColorコンポーネントを管理するマネージャ
 * @author suzuki.shunsuke
 * @date 2022/02/05
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Component/ComponentManager.h"
#include "ElevationColor.h"

 /**
 * @brief ElevationColorコンポーネントを管理するマネージャ
 */
class ElevationColorManager 
	: public SingletonFounder<ElevationColorManager>
	, public ComponentManager<ElevationColor>
{
	friend SingletonFounder;

private:
	/**
	* @brief コンストラクタ
	*/
	ElevationColorManager();

	/**
	* @brief デストラクタ
	*/
	~ElevationColorManager() = default;

private:
	/** 最大標高 */
	float mMaxHeight;

	/** 山頂色 */
	DirectX::SimpleMath::Vector3 mTopColor;
	/** 地面色 */
	DirectX::SimpleMath::Vector3 mGroundColor;

public:
	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update() override;

public:

};