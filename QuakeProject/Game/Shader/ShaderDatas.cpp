/**
 * @file ShaderDatas.cpp
 * @brief �V�F�[�_���g�����ɕK�v�ȃf�[�^�Q
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
#include "pch.h"
#include "ShaderDatas.h"
#include "DeviceResources.h"
#include "BinaryFile.h"
#include "Libraries/MyLibraries/Utility.h"

using namespace DirectX;

 //=====================================================
 //		������
 //=====================================================
void ShaderDataPack::Initialize(const wchar_t* VSpath, const wchar_t* PSpath)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	// �v���~�e�B�u�o�b�`�̍쐬
	mPrimitiveBatch = std::make_unique<
		PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// cso�t�@�C���^�C�v�ɉ����Ă�ǂݎ��
	BinaryFile VSData = BinaryFile::LoadFile(VSpath);
	BinaryFile PSData = BinaryFile::LoadFile(PSpath);

	// ���̓��C�A�E�g��ݒ�
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		mInputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL,
		mVertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		assert(L"CreateVertexShader Failed.");
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL,
		mPixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		assert(!L"CreatePixelShader Failed.");
	}

	// �萔�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &mCBuffer);

	// �R�����X�e�[�g�ݒ�
	mStates = std::make_unique<CommonStates>(device);
}
