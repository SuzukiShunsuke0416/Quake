/**
 * @file ShaderDatas.cpp
 * @brief シェーダを使う時に必要なデータ群
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
 //		初期化
 //=====================================================
void ShaderDataPack::Initialize(const wchar_t* VSpath, const wchar_t* PSpath)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	// プリミティブバッチの作成
	mPrimitiveBatch = std::make_unique<
		PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// csoファイルタイプに応じてを読み取る
	BinaryFile VSData = BinaryFile::LoadFile(VSpath);
	BinaryFile PSData = BinaryFile::LoadFile(PSpath);

	// 入力レイアウトを設定
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		mInputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL,
		mVertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		assert(L"CreateVertexShader Failed.");
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL,
		mPixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		assert(!L"CreatePixelShader Failed.");
	}

	// 定数バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &mCBuffer);

	// コモンステート設定
	mStates = std::make_unique<CommonStates>(device);
}
