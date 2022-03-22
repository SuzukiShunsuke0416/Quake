/**
 * @file StagePoint.h
 * @brief �X�e�[�W���\������P�̓_�i�A�N�^�[�j
 * @author suzuki.shunsuke
 * @date 2022/02/02
 */
#pragma once
#include "Game/Actor/Actor.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Game/ElevationColor/ElevationColor.h"
#include "Libraries/MyLibraries/MyVertexTypes.h"
#include <unordered_set>
#include "Libraries/MyLibraries/PerlinNoise.h"

class StagePoint;

/**
* @brief StagePoint�̏������ɕK�v�ȏ��Q
*/
struct StagePointInitDesc {
	DirectX::SimpleMath::Vector3 pos;
	Int2 element;
	StagePoint* nextRight;
	StagePoint* nextBottom;
	StagePoint* nextRightBottom;
	DataForPrimitiveBatchRender<DirectX::VertexPositionColorTexture>* 
		pLineBatchDatas;
};

/**
* @brief �X�e�[�W���\������P�̓_
*/
class StagePoint :public Actor{
public:

private:
	/** �����̔Ԓn(�z��̗v�f)�ԍ� */
	Int2 mElement;

	/** �אړ_��ptr�i�E�j */
	StagePoint* mpNextRight;
	/** �אړ_��ptr�i���j */
	StagePoint* mpNextBottom;
	/** �אړ_��ptr�i�E���j */
	StagePoint* mpNextRightBottom;

	/** �אړ_��ptr���܂Ƃ߂�����*/
	std::unordered_set<StagePoint*> mpNextPoints;

	/** �F���*/
	ElevationColor* mpColor;

	/** �v���~�e�B�u�o�b�`�f�[�^�Q*/
	DataForPrimitiveBatchRender<DirectX::VertexPositionColorTexture>* 
		mpPrimitiveBatchDatas;
public:

	/**
	* @brief �R���X�g���N�^
	*/
	StagePoint();
	/**
	* @brief �f�X�g���N�^
	*/
	~StagePoint();

	/**
	 * @brief	������
	 * @param[in] desc	�������f�[�^�Q
	 * @param[in] noiseMaker �m�C�Y�����@
	 */
	void Initialize(const StagePointInitDesc& desc, PerlinNoise& noiseMaker);

	/**
	* @brief �X�V
	*/
	void Update() override;

	/**
	* @brief �ʂ̕`��
	*/
	void Render() override;

	/**
	* @brief ���̕`��
	*/
	void LineRender();

	/**
	* @brief �����i���W�j���擾
	*/
	float GetLocationY() {
		return mTransform.Location.y;
	}

	/**
	* @brief �F���̎擾
	*/
	ElevationColor* GetElevationColor() {
		return mpColor;
	}
};