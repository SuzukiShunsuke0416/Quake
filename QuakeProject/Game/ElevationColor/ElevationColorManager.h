/**
 * @file ElevationColorManager.h
 * @brief ElevationColor�R���|�[�l���g���Ǘ�����}�l�[�W��
 * @author suzuki.shunsuke
 * @date 2022/02/05
 */
#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include "Game/Component/ComponentManager.h"
#include "ElevationColor.h"

 /**
 * @brief ElevationColor�R���|�[�l���g���Ǘ�����}�l�[�W��
 */
class ElevationColorManager 
	: public SingletonFounder<ElevationColorManager>
	, public ComponentManager<ElevationColor>
{
	friend SingletonFounder;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	ElevationColorManager();

	/**
	* @brief �f�X�g���N�^
	*/
	~ElevationColorManager() = default;

private:
	/** �ő�W�� */
	float mMaxHeight;

	/** �R���F */
	DirectX::SimpleMath::Vector3 mTopColor;
	/** �n�ʐF */
	DirectX::SimpleMath::Vector3 mGroundColor;

public:
	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	void Update() override;

public:

};