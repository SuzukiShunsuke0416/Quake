/**
 * @file CaterpillarShaft.h
 * @brief �L���^�s���N���X
 * @author suzuki.shunsuke
 * @date ???
 */
#pragma once
#include "Caterpillar.h"
#include <vector>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include "Libraries/MyLibraries/MyVertexTypes.h"
#include "Game/ScreenEffect/BrightChanger.h"

 /**
  * @brief �L���^�s�����ǂ��瑤�ɂ��邩
  */
enum class eCaterSideType {
	Left = -1,
	Right = 1,
};

class PlayerVisual;
/**
 * @brief �L���^�s���������f�[�^�Q
 */
struct Cater_Init_Desc {
	PlayerVisual* pOwner;
	eCaterSideType sideType;

	Cater_Init_Desc() 
		:pOwner()
		,sideType()
	{
	}
};

/**
 * @brief �L���^�s���N���X
 */
class CaterpillarShaft{
public:
	

private:
	/** ������ */
	PlayerVisual* mpOwner;

	/** ��]���x */
	float mRotVel;

	/** �L���^�s������ */
	std::vector<std::unique_ptr<CaterpillarParts>> 
		mCaterpillars;

	/** �L���^�s���|���S���̌^ */
	using CaterpillarPolygonType = DataForPrimitiveBatchRender<DirectX::VertexPositionColor>;

	/** �L���^�s���`��p�f�[�^�Q */
	std::unique_ptr<CaterpillarPolygonType> 
		mpCaterpillarPolygonData;

	/** ���E��� */
	eCaterSideType mSideType;

	/** ���点�S�� */
	std::unique_ptr<BrightChanger> mpBright;

private: // �O���ǂݍ��݌n

	/** �L���^�s��(�S��)�̔��a */
	float mHalfSize;

	/** �L���^�s���̏c����i�����c�̉��{���j */
	float mSideRat;

	/** �L���^�s���̐� */
	int mCaterNum;

	/** �L���^�s��(��)�̉��� */
	float mHalfWidthSize;

	/** ��Ԃ���̋��� */
	float mDistanceFromTank;

public:
	/**
	* @brief �R���X�g���N�^
	* @param[in] cid Cater_Init_Desc ���N���X�������p�f�[�^�Q
	*/
	CaterpillarShaft(const Cater_Init_Desc& cid);

	/**
	* @brief �f�X�g���N�^
	*/
	~CaterpillarShaft();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

private:

	/**
	* @brief �O���ǂݍ��݌n�f�[�^�̏�����
	*/
	void InitExternalData();

public:
	/**
	* @brief ���a�̎擾
	*/
	inline float GetHalfSize() {
		return mHalfSize;
	}
	/**
	* @brief �c����̎擾
	*/
	inline float GetSideRat() {
		return mSideRat;
	}
	/**
	* @brief �L���^�s���̐��̎擾
	*/
	inline int GetCaterNum() {
		return mCaterNum;
	}
};