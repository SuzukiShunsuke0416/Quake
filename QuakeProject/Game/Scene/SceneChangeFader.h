/**
 * @file SceneChangeFader.h
 * @brief シーン切り替え時のフェードを担当するクラス
 * @author suzuki.shunsuke
 * @date 2022/01/30
 */
#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <unordered_map>
#include <functional>

 /**
 * @brief フェードの種類
 */
enum class eSceneChangeFadeType :unsigned int{
	Normal,

	TYPE_NUM
};

/**
 * @brief シーン切り替え時のフェードを担当するクラス
 */
class SceneChangeFader {
private:
	/** 個別更新関数の型 */
	using updateMold = std::function<bool(SceneChangeFader*)>;
	/** 更新処理をまとめた配列（静的） */
	static const updateMold 
		msUpdateFunctions[(unsigned int)(eSceneChangeFadeType::TYPE_NUM)];

	/** 通常パラメータの終了値 */
	static const float NORMAL_PARAM_END;

private:
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

	/** 各オブジェクトが担当する更新関数 */
	updateMold mUpdate;

public:
	/**
	* @brief コンストラクタ
	*/
	SceneChangeFader();
	/**
	* @brief デストラクタ
	*/
	~SceneChangeFader() = default;

	/**
	 * @brief 初期化関数
	 * @param[in] type フェードタイプ
	 */
	void Initialize(eSceneChangeFadeType type);

	/**
	 * @brief 更新
	 * @return フェードが終わったかどうか
	 */
	bool Update();

	/**
	 * @brief 描画
	 * @param[in] srv マスク用シェーダリソースビュー
	 */
	void Draw(ID3D11ShaderResourceView* srv);

private:
	/**
	 * @brief 通常更新関数　（１つのパラメータが等速で増えるのみ）
	 * @return 値が終了値になったか
	 */
	bool NormalUpdate();
};