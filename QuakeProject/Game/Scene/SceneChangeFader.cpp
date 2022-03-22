/**
 * @file SceneChangeFader.cpp
 * @brief シーン切り替え時のフェードを担当するクラス
 * @author suzuki.shunsuke
 * @date 2022/01/30
 */
#include "pch.h"
#include "SceneChangeFader.h"
#include "DeviceResources.h"
#include "Game/Shader/BinaryFile.h"
#include "Libraries/MyLibraries/Utility.h"
#include <map>
#include "Game/Scene/SceneManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 更新関数(定数)を設定
const std::function<bool(SceneChangeFader*)> SceneChangeFader::msUpdateFunctions[] =
{ 
	std::mem_fn(&SceneChangeFader::NormalUpdate),//通常更新関数 (Normal)
};

// 定数値設定
const float SceneChangeFader::NORMAL_PARAM_END = 1.0f;

//=====================================================
//		コンストラクタ
//=====================================================
SceneChangeFader::SceneChangeFader()
	:mTimeParam()
	,mPrimitiveBatch()
	,mVertexShader()
	,mPixelShader()
	,mCBuffer()
	,mStates()
	,mInputLayout()
	,mUpdate()
{
}

//=====================================================
//		初期化
//=====================================================
void SceneChangeFader::Initialize(eSceneChangeFadeType type)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	// プリミティブバッチの作成
	mPrimitiveBatch = std::make_unique<
		PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// csoまでのパスを一括管理
	std::map<eSceneChangeFadeType, wchar_t*> shaderPathsVS;
	std::map<eSceneChangeFadeType, wchar_t*> shaderPathsPS;
	shaderPathsVS[eSceneChangeFadeType::Normal] = L"Resources/Shaders/ParticleVS.cso";
	shaderPathsPS[eSceneChangeFadeType::Normal] = L"Resources/Shaders/ParticlePS.cso";

	// csoファイルタイプに応じてを読み取る
	BinaryFile VSData = BinaryFile::LoadFile(shaderPathsVS[type]);
	BinaryFile PSData = BinaryFile::LoadFile(shaderPathsPS[type]);

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

	// 更新関数設定
	mUpdate = msUpdateFunctions[(unsigned int)(type)];
}

//=====================================================
//		更新
//=====================================================
bool SceneChangeFader::Update()
{
	return mUpdate(this);
}

//=====================================================
//		描画
//=====================================================
void SceneChangeFader::Draw(ID3D11ShaderResourceView* srv)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	//シェーダに渡すパラメータの設定
	ConstBuffer cbuff;
	cbuff.matView = Matrix::Identity.Transpose();
	cbuff.matProj = Matrix::Identity.Transpose();
	cbuff.matWorld = Matrix::Identity.Transpose();
	cbuff.Time = mTimeParam;

	//定数バッファの内容更新
	context->UpdateSubresource(mCBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = mStates->NonPremultiplied();
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(mStates->DepthRead(), 0);
	// カリング
	context->RSSetState(mStates->CullNone());

	//定数バッファをシェーダに渡す
	ID3D11Buffer* cb[1] = { mCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);


	//サンプラー、シェーダ、画像をそれぞれ登録
	ID3D11SamplerState* sampler[1] = { mStates->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, &srv);

	//入力レイアウトを反映
	context->IASetInputLayout(mInputLayout.Get());

	// 名前が長すぎるので一時的に省略
	using VertexPCT = VertexPositionColorTexture;

	VertexPCT ver[] = {
		VertexPCT(Vector3(-1.0f,1.0f,0.0f),Vector4::Zero,Vector2(0.0f,0.0f)),
		VertexPCT(Vector3(1.0f,1.0f,0.0f),Vector4::Zero,Vector2(1.0f,0.0f)),
		VertexPCT(Vector3(1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(1.0f,1.0f)),
		VertexPCT(Vector3(-1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(0.0f,1.0f))
	};

	//バッチに頂点情報を渡す
	mPrimitiveBatch->Begin();
	mPrimitiveBatch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	mPrimitiveBatch->End();


	//他のモデルに影響が出る可能性があるので使い終わったらシェーダを外す
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

//=====================================================
//		更新【通常】
//=====================================================
bool SceneChangeFader::NormalUpdate()
{
	mTimeParam.x += SceneManager::GetInstance()->GetElapsedTime();

	DebugLogManager::GetInstance()->EntryLog(mTimeParam.x,L"timeVal");

	return mTimeParam.x >= NORMAL_PARAM_END;
}
