#pragma once
#include <PrimitiveBatch.h>
/**
 * @file ShaderDatas.h
 * @brief シェーダを使う時に必要なデータ群
 * @author suzuki.shunsuke
 * @date 2022/03/13
 */
struct ShaderDataPack {
	/** シェーダに渡す自由枠の値 */
	DirectX::SimpleMath::Vector4 mTimeParam;

	/** プリミティブバッチ */
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>
		mPrimitiveBatch;

	/** 頂点シェーダ */
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;

	/** ピクセルシェーダ */
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;

	/** 定数バッファ */
	Microsoft::WRL::ComPtr<ID3D11Buffer> mCBuffer;

	/** コモンステート */
	std::unique_ptr<DirectX::CommonStates> mStates;

	/** 入力レイアウト */
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	/**
	* @brief コンストラクタ
	*/
	ShaderDataPack()
		: mTimeParam()
		, mPrimitiveBatch()
		, mVertexShader()
		, mPixelShader()
		, mCBuffer()
		, mStates()
		, mInputLayout() 
	{
	}

	/**
	* @brief 初期化
	* @param[in] 頂点シェーダのパス
	* @param[in] ピクセルシェーダのパス
	*/
	void Initialize(const wchar_t* VSpath,const wchar_t* PSpath);
};