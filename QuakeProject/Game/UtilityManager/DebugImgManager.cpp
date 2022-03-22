#include "pch.h"
#include "DebugImgManager.h"
#include "DeviceResources.h"
#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		�R���X�g���N�^
//=====================================================
DebugImgManager::DebugImgManager()
	:mSpriteBatch()
	,mTextures{}
{
	//�z��̋�v�f������Ă���
	for (int i = 0; i < (int)(eDebugImgType::TYPE_NUM); i++) {
		mTextures[(eDebugImgType)(i)] = nullptr;
		mPositions[(eDebugImgType)(i)] = std::vector<std::pair<int, int>>();
	}


	//�X�v���C�g�o�b�`�̐ݒ�
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	ID3D11Device1* device = pDR->GetD3DDevice();
	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	//Texture�̐ݒ�
	std::unordered_map<eDebugImgType, wchar_t*> paths;
	paths[eDebugImgType::Full1] = L"Resources/Textures/DebugTitle.png";
	paths[eDebugImgType::Full2] = L"Resources/Textures/DebugImage2.png";
	paths[eDebugImgType::TridentLogo] = L"Resources/Textures/TridentLogo.png";
	for (int i = 0; i < (int)(eDebugImgType::TYPE_NUM); i++) {
		CreateWICTextureFromFile(
			device,
			paths[(eDebugImgType)(i)],
			nullptr,
			mTextures[(eDebugImgType)(i)].ReleaseAndGetAddressOf()
		);

		if (mTextures[(eDebugImgType)(i)] == nullptr) {
			assert(!"Texture�̓ǂݍ��݂Ɏ��s");
		}
	}

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	mCommonState = std::make_unique<DirectX::CommonStates>(device);
}

//=====================================================
//		�`��
//=====================================================
void DebugImgManager::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	RECT rect = pDR->GetOutputSize();

	for (int i = 0; i < (int)(eDebugImgType::TYPE_NUM); i++)
	{
		for (auto& pos : mPositions[(eDebugImgType)(i)]) 
		{
			mSpriteBatch->Begin(SpriteSortMode_Deferred, mCommonState->NonPremultiplied());

			if ((eDebugImgType)(i) == eDebugImgType::Full1 ||
				(eDebugImgType)(i) == eDebugImgType::Full2) 
			{
				mSpriteBatch->Draw(mTextures[(eDebugImgType)(i)].Get(), rect);
			}
			else {
				mSpriteBatch->Draw(mTextures[(eDebugImgType)(i)].Get(),
					XMFLOAT2((float)(pos.first), (float)(pos.second)));
			}

			mSpriteBatch->End();
		}
	}

	this->Reset();
}

//=====================================================
//		�ǉ�
//=====================================================
void DebugImgManager::Entry(eDebugImgType type, int x, int y)
{
	mPositions[type].push_back({ x,y });
}

//=====================================================
//		�u�ԕ`��
//=====================================================
void DebugImgManager::QuickDraw(eDebugImgType type, int x, int y)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	RECT rect = pDR->GetOutputSize();

	mSpriteBatch->Begin(SpriteSortMode_Deferred, mCommonState->NonPremultiplied());

	if (type == eDebugImgType::Full1 || type == eDebugImgType::Full2)
	{
		mSpriteBatch->Draw(mTextures[type].Get(), rect);
	}
	else {
		mSpriteBatch->Draw(mTextures[type].Get(), XMFLOAT2((float)x,(float)y));
	}

	mSpriteBatch->End();
}

//=====================================================
//		���W�z��̃��Z�b�g
//=====================================================
void DebugImgManager::Reset()
{
	for (auto& pos : mPositions) {
		pos.second.clear();
	}
}
