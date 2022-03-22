#include "pch.h"
#include "ScreenEffectDatas.h"
#include "DeviceResources.h"

//=====================================================
//		コンストラクタ
//=====================================================
ScreenEffectDatas::ScreenEffectDatas()
	:mTexture2D()
	, mRenderTargetView()
	, mShaderResourceView()
{
	// デバイスリソースを取得
	auto pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();

	// レンダリングターゲット用のテクスチャを作成
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = pDR->GetOutputSize().right;
	texDesc.Height = pDR->GetOutputSize().bottom;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	device->CreateTexture2D(&texDesc, NULL, mTexture2D.GetAddressOf());

	// レンダーターゲットビューの生成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(mTexture2D.Get(), &rtvDesc, mRenderTargetView.GetAddressOf());

	// シェーダーリソースビューの生成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(mTexture2D.Get(), &srvDesc, mShaderResourceView.GetAddressOf());
}
