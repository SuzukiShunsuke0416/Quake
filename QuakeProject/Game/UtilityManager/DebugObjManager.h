/**
 * @file DebugObjManager.h
 * @brief 3D�I�u�W�F�N�g��`�悷���p�̃}�l�[�W���[
 * @author suzuki.shunsuke
 * @date 2022/02/27
 */
#pragma once
#include "SingletonFounder.h"
#include <vector>
#include <GeometricPrimitive.h>
#include <SimpleMath.h>

/**
 * @brief 3D�I�u�W�F�N�g��`�悷���p�̃}�l�[�W���[
 */
class DebugObjManager :public SingletonFounder<DebugObjManager> {
public:
	friend SingletonFounder;

private:
	/**
	 * @brief �f�o�b�O�I�u�W�F�i�X�t�B�A�j�p�̃f�[�^�Q
	 */
	struct DebugObjData_Sphere {
		DirectX::SimpleMath::Vector3 pos;
		float scale;
		DirectX::SimpleMath::Vector3 color;
	};

	/**
	 * @brief �f�o�b�O�I�u�W�F�i�L���[�u�j�p�̃f�[�^�Q
	 */
	struct DebugObjData_Cube {
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Vector3 color;
	};

	/**
	 * @brief  �f�o�b�O�I�u�W�F�i�e�B�[�|�b�g�j�p�̃f�[�^�Q�i�ʖ���`�j
	 */
	using DebugObjData_TeaPot = DebugObjData_Cube;


	/** �f�o�b�O�`��f�[�^�i�X�t�B�A�j�p�z�� */
	std::vector<DebugObjData_Sphere> mSphereDataVec;
	/** �f�o�b�O�`��f�[�^�i�L���[�u�j�p�z�� */
	std::vector<DebugObjData_Cube> mCubeDataVec;
	/** �f�o�b�O�`��f�[�^�i�e�B�[�|�b�g�j�p�z�� */
	std::vector<DebugObjData_TeaPot> mTeaPotDataVec;

	/** �W�I���g���b�N�v���~�e�B�u�i�X�t�B�A�p�j */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPri;
	/** �W�I���g���b�N�v���~�e�B�u�i�L���[�u�p�j */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPriCube;
	/** �W�I���g���b�N�v���~�e�B�u�i�e�B�[�|�b�g�p�j */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPriTeaPot;
private:
	/**
	* @brief �R���X�g���N�^
	*/
	DebugObjManager();

public:
	/**
	* @brief �f�X�g���N�^
	*/
	~DebugObjManager() {

	}

	/**
	* @brief �`��
	 * @param[in] view	�r���[�s��
	 * @param[in] proj	�v���W�F�N�V�����s��
	*/
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj);

	/**
	 * @brief	�`��\���i�X�t�B�A�j
	 * @param[in]	pos		���W
	 * @param[in]	scale	�X�P�[��
	 * @param[in]	color	�F
	 */
	inline void Entry(const DirectX::SimpleMath::Vector3& pos,
		float scale = 1.0f,
		DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Vector3::One)
	{
		mSphereDataVec.push_back({pos,scale,color });
	}

	/**
	 * @brief	�`��\���i�L���[�u�j
	 * @param[in]	world	���f���s��
	 * @param[in]	color	�F
	 */
	inline void EntryCube(const DirectX::SimpleMath::Matrix& world,
		DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Vector3::One)
	{
		mCubeDataVec.push_back({ world,color });
	}

	/**
	 * @brief	�`��\���i�e�B�[�|�b�g�j
	 * @param[in]	world	���f���s��
	 * @param[in]	color	�F
	 */
	inline void EntryTeaPot(const DirectX::SimpleMath::Matrix& world,
		DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Vector3::One)
	{
		mTeaPotDataVec.push_back({ world,color });
	}

private:

	/**
	* @brief �z�����ɂ���
	*/
	inline void Clear() {
		mSphereDataVec.clear();
		mCubeDataVec.clear();
		mTeaPotDataVec.clear();
	}
};