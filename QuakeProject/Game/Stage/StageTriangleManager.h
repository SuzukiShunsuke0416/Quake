/**
 * @file StageTriangleManager.h
 * @brief �X�e�[�W�g���C�A���O���R���|�[�l���g���Ǘ�����B
 * @author suzuki.shunsuke
 * @date 2022/02/011
 */
#pragma once
#include "Game/Component/ComponentManager.h"
#include "StageTriangle.h"

 /**
 * @brief StageTriangle���Ǘ�����B
 */
class StageTriangleManager :public ComponentManager<StageTriangle> {

public:
	/**
	* @brief �R���X�g���N�^
	*/
	StageTriangleManager();
	/**
	* @brief �f�X�g���N�^
	*/
	~StageTriangleManager();


public:
	/**
	* @brief �����o�[�z��̎擾
	* @note �e�N���X��ComponentManager�ɁA
			�z�񎩑̂��擾����֐��͑��݂��Ȃ����ߎ����B
	*/
	const std::vector<StageTriangle*>& GetStageTriangles() {
		return mpMembers;
	}
};