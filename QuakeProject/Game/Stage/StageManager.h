/**
 * @file StageManager.h
 * @brief �X�e�[�W�����Ǘ�����N���X
 * @author suzuki.shunsuke
 * @date 2022/02/03
 */
#pragma once
#include <vector>
#include "StagePoint.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Game/UtilityManager/SingletonFounder.h"
#include "Libraries/MyLibraries/MyVertexTypes.h"
#include "StageTriangle.h"
#include <unordered_map>
#include "Game/Collision/SimpleCheckFunctions.h"
#include "Game/ScreenEffect/BrightChanger.h"

class StageTriangleManager;
/**
 * @brief �X�e�[�W�����Ǘ�����N���X
 */
class StageManager :public SingletonFounder<StageManager> {
	friend SingletonFounder;
public:
	
private:
	/** �X�e�[�W�_��� */
	std::vector<std::vector<StagePoint*>> mpStagePoints;

	/** �X�e�[�W�T�C�Y */
	Int2 mStageSise;

	/** �_�Ɠ_�̋��� = �X�e�[�W�̍L�� */
	float mPointDistance;

	/** �ő�W�� */
	float mMaxHeight;

	/** �R���F */
	ColorData mTopColor;
	/** �n�ʐF */
	ColorData mGroundColor;
	
	/** �v���~�e�B�u�o�b�`�`��̂��߂̃f�[�^�Q */
	DataForPrimitiveBatchRender<DirectX::VertexPositionColorTexture>*
		mpLineBatchDatas;

	/** �X�e�[�W�̒����n�_ */
	DirectX::SimpleMath::Vector3 mCenterPosition;

	/** StageTriangle�̃}�l�[�W�� */
	StageTriangleManager* mpStageTriangleManager;

	/** �X�e�[�W�̒[���W�i4�Ӂj */
	BoxParam<float> mStageSidePosBox;

	/** ���点�S�� */
	std::unique_ptr<BrightChanger> mpBright;

	/** �X�e�[�W�|���S���`�掞�Ɏg���Ăяo���� */
	std::vector<uint16_t> mIndices;

	/** �X�e�[�W�|���S���`�掞�Ɏg�����_��� */
	std::vector<DirectX::VertexPositionColorTexture>
		mVertices;
private:

	/**
	* @brief �R���X�g���N�^
	*/
	StageManager();

	/**
	* @brief �f�X�g���N�^
	*/
	~StageManager();

public:
	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

	/**
	* @brief �I������
	*/
	void Finalize();

public:
	/**
	 * @brief	StageTriangle�����o�̒ǉ�
	 * @param[in] triangle	�ǉ������ײ�ݸ��
	 */
	void AddTriangleMember(StageTriangle* triangle);
	/**
	 * @brief	StageTriangle�����o�̏��O
	 * @param[in] triangle	���O�����ײ�ݸ��
	 */
	void RemoveTriangleMember(StageTriangle* triangle);
	/**
	 * @brief	StageTriangleManager�̎擾
	 * @return	StageTriangleManager�̃|�C���^
	 */
	inline StageTriangleManager* GetStageTriangleManager() {
		return mpStageTriangleManager; 
	}

private:

	/**
	 * @brief	StageTriangle�����̒l�ݒ�
	 */
	void CalculateStageTriangle();

	/**
	 * @brief	�ʕ`�掞�Ɏg���l�����̐ݒ�
	 */
	void SetPolygonDatas();
public:
	/**
	 * @brief	�ő�W���̎擾
	 */
	inline float GetMaxHeight() { return mMaxHeight; }

	/**
	 * @brief	�R���F�̎擾
	 */
	inline const ColorData& GetTopColor() { return mTopColor; }

	/**
	 * @brief	�n�ʐF�̎擾
	 */
	inline const ColorData& GetGroundColor() { return mGroundColor; }

	/**
	 * @brief	�������W�̎擾
	 */
	inline const DirectX::SimpleMath::Vector3& GetCenterPos() { return mCenterPosition; }

	/**
	 * @brief	�X�e�[�W�̒[���W�̎擾
	 */
	inline const BoxParam<float>& GetSidePosBox() { return mStageSidePosBox; }
	
	/**
	 * @brief	�S�X�e�[�W�|�C���g�̎擾
	 */
	inline const std::vector<std::vector<StagePoint*>>& GetAllStagePoints() 
	{ return mpStagePoints; }

	/**
	 * @brief	�X�e�[�W�T�C�Y�i�v�f���j�̎擾
	 */
	inline Int2 GetStageSize() { return mStageSise; }
};