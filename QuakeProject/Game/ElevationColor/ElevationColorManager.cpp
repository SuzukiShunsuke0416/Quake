/**
 * @file ElevationColorManager.cpp
 * @brief ElevationColor�R���|�[�l���g���Ǘ�����}�l�[�W��
 * @author suzuki.shunsuke
 * @date 2022/02/05
 */
#include "pch.h"
#include "ElevationColorManager.h"
#include "Game/Stage/StageManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
ElevationColorManager::ElevationColorManager()
	:ComponentManager()
	,mMaxHeight()
{
}

//=====================================================
//		������
//=====================================================
void ElevationColorManager::Initialize()
{
	//�X�e�[�W�}�l�[�W������A�ő�W���ƎR���E�n�ʐF���擾����B
	auto stageManager = StageManager::GetInstance();
	ColorData topColorData = stageManager->GetTopColor();
	ColorData groundColorData = stageManager->GetGroundColor();

	//�ő�W���A�F��ݒ肷��B
	mMaxHeight = stageManager->GetMaxHeight();
	mTopColor = Vector3(topColorData.r, topColorData.g, topColorData.b);
	mGroundColor = Vector3(groundColorData.r, groundColorData.g, groundColorData.b);
}

//=====================================================
//		�X�V
//=====================================================
void ElevationColorManager::Update()
{
	for (const auto color : mpMembers) {
		float heightRat = color->GetOwner()->GetTransform().Location.y / mMaxHeight;

		color->SetColorV3(myf::Lerp(mGroundColor, mTopColor, heightRat));
	}
}
