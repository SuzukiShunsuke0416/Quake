#include "pch.h"
#include "ScreenEffectBase.h"
#include "DeviceResources.h"
#include "Libraries/MyLibraries/Utility.h"
#include "Game/Scene/SceneManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
ScreenEffectBase::ScreenEffectBase()
	:mpScreenEffectDatas()
	, mDefaultRTV()
	, mShaderDataPack()
{
	// 画面効果用データ群の生成
	mpScreenEffectDatas = std::make_unique<ScreenEffectDatas>();
}

//=====================================================
//		描画効果開始
//=====================================================
void ScreenEffectBase::Begin()
{
	// デフォルトの描画設定達を呼び出す
	auto pDR = DX::DeviceResources::GetInstance();
	//mDefaultRTV = pDR->GetRenderTargetView();
	mDefaultRTV = SceneManager::GetInstance()->GetCurrentRTV();
	auto depthStencil = pDR->GetDepthStencilView();

	// レンダリング先をこのクラスが用意した退避用に切り替える
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		mpScreenEffectDatas->mRenderTargetView.GetAddressOf(),
		depthStencil
	);

}

//=====================================================
//		描画効果終了
//=====================================================
void ScreenEffectBase::End(bool isBlendAdd)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	auto depthStencil = pDR->GetDepthStencilView();

	// レンダーターゲットを元に戻す
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		&mDefaultRTV,
		depthStencil);

	// 加算合成？
	if (isBlendAdd) {
		ID3D11BlendState* blendstate = mShaderDataPack.mStates->Additive();
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	}
	else {
		ID3D11BlendState* blendstate = mShaderDataPack.mStates->NonPremultiplied();
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	}
	

	// 発光描画
	this->Render();

	FLOAT color[] = { 0,0,0,0 };

	// 自前レンダーターゲットビューをクリア
	context->ClearRenderTargetView(
		mpScreenEffectDatas->mRenderTargetView.Get(),
		color
	);

}

//=====================================================
//		効果本描画共通処理
//=====================================================
void ScreenEffectBase::Render()
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	// 画面比を計算
	RECT screenSize = pDR->GetOutputSize();
	mShaderDataPack.mTimeParam.w 
		= (float)(screenSize.bottom) / (float)(screenSize.right);

	//シェーダに渡すパラメータの設定
	ConstBuffer cbuff;
	cbuff.matView = Matrix::Identity.Transpose();
	cbuff.matProj = Matrix::Identity.Transpose();
	cbuff.matWorld = Matrix::Identity.Transpose();
	cbuff.Time = mShaderDataPack.mTimeParam;

	//定数バッファの内容更新
	context->UpdateSubresource(mShaderDataPack.mCBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(mShaderDataPack.mStates->DepthRead(), 0);
	// カリング
	context->RSSetState(mShaderDataPack.mStates->CullNone());

	//定数バッファをシェーダに渡す
	ID3D11Buffer* cb[1] = { mShaderDataPack.mCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);


	//サンプラー、シェーダ、画像をそれぞれ登録
	ID3D11SamplerState* sampler[1] = { mShaderDataPack.mStates->LinearClamp() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(mShaderDataPack.mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mShaderDataPack.mPixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, mpScreenEffectDatas->mShaderResourceView.GetAddressOf());

	//入力レイアウトを反映
	context->IASetInputLayout(mShaderDataPack.mInputLayout.Get());

	// 名前が長すぎるので一時的に省略
	using VertexPCT = VertexPositionColorTexture;

	// 全画面
	VertexPCT ver[] = {
		VertexPCT(Vector3(-1.0f,1.0f,0.0f),Vector4::Zero,Vector2(0.0f,0.0f)),
		VertexPCT(Vector3(1.0f,1.0f,0.0f),Vector4::Zero,Vector2(1.0f,0.0f)),
		VertexPCT(Vector3(1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(1.0f,1.0f)),
		VertexPCT(Vector3(-1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(0.0f,1.0f))
	};

	//バッチに頂点情報を渡す
	mShaderDataPack.mPrimitiveBatch->Begin();
	mShaderDataPack.mPrimitiveBatch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	mShaderDataPack.mPrimitiveBatch->End();


	//他のモデルに影響が出る可能性があるので使い終わったらシェーダを外す
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
