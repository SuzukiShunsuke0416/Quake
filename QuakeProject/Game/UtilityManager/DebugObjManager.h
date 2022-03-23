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
#include "Libraries/MyLibraries/MyVertexTypes.h"

/**
 * @brief 3D�I�u�W�F�N�g��`�悷���p�̃}�l�[�W���[
 */
class DebugObjManager :public SingletonFounder<DebugObjManager> {
public:
	friend SingletonFounder;

private:
	/**
	 * @brief �X�t�B�A�p�̃f�[�^�Q
	 */
	struct DebugObjData_Sphere {
		DirectX::SimpleMath::Vector3 pos;
		float scale;
		DirectX::SimpleMath::Vector3 color;
	};

	/**
	 * @brief �L���[�u�p�̃f�[�^�Q
	 */
	struct DebugObjData_Cube {
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Vector3 color;
	};

	/**
	 * @brief  �e�B�[�|�b�g�p�̃f�[�^�Q�i�ʖ���`�j
	 */
	using DebugObjData_TeaPot = DebugObjData_Cube;

	/**
	 * @brief ���C���p�̃f�[�^�Q
	 */
	struct DebugObjData_Line {
		DirectX::SimpleMath::Vector3 StartPos;
		DirectX::SimpleMath::Vector3 EndPos;
		DirectX::SimpleMath::Vector4 StartColor;
		DirectX::SimpleMath::Vector4 EndColor;
	};


	/** �X�t�B�A�`��f�[�^�p�z�� */
	std::vector<DebugObjData_Sphere> mSphereDataVec;
	/** �L���[�u�`��f�[�^�p�z�� */
	std::vector<DebugObjData_Cube> mCubeDataVec;
	/** �e�B�[�|�b�g�`��f�[�^�p�z�� */
	std::vector<DebugObjData_TeaPot> mTeaPotDataVec;
	/** ���C���`��f�[�^�p�z�� */
	std::vector<DebugObjData_Line> mLineDataVec;

	/** �W�I���g���b�N�v���~�e�B�u�i�X�t�B�A�p�j */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPri;
	/** �W�I���g���b�N�v���~�e�B�u�i�L���[�u�p�j */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPriCube;
	/** �W�I���g���b�N�v���~�e�B�u�i�e�B�[�|�b�g�p�j */
	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPriTeaPot;
	/** �v���~�e�B�u�o�b�`�i���C���p�j */
	std::unique_ptr<DataForPrimitiveBatchRender<DirectX::VertexPositionColor>>
		mpPriLine;

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

	/**
	 * @brief	�`��\���i���C���j
	 * @param[in]	startPos	�n�_
	 * @param[in]	endPos		�I�_
	 * @param[in]	startColor	�n�_�F
	 * @param[in]	endColor	�I�_�F
	 */
	inline void EntryLine(
		const DirectX::SimpleMath::Vector3& startPos,
		const DirectX::SimpleMath::Vector3& endPos,
		const DirectX::SimpleMath::Vector4& startColor = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		const DirectX::SimpleMath::Vector4& endColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f,1.0f)) 
	{
		mLineDataVec.push_back({ startPos,endPos,startColor,endColor });
	}

private:

	/**
	* @brief �z�����ɂ���
	*/
	inline void Clear() {
		mSphereDataVec.clear();
		mCubeDataVec.clear();
		mTeaPotDataVec.clear();
		mLineDataVec.clear();
	}
};