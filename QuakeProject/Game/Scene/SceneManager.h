/**
 * @file SceneManager.h
 * @brief 各シーンを管理するクラス
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */

#pragma once
#include "Game/UtilityManager/SingletonFounder.h"
#include <vector>
#include <functional>
#include <StepTimer.h>

 /**
  * @enum eSceneType
  * シーンのタイプ
  */
enum class eSceneType {
	Title,
	Play,
};

class IScene;
class TitleScene;
class PlayScene;

/**
* @brief 各シーンを管理するクラス
*/
class SceneManager :public SingletonFounder<SceneManager> {
	friend SingletonFounder;
private:
	/**
	* @brief コンストラクタ
	*/
	SceneManager();

	/**
	* @brief デストラクタ
	*/
	~SceneManager();
public:
	/** 同時に処理される更新関数の数 */
	static const int MAX_FUNCTION_COUNT;

private:
	/** 直前の1f経過時間 */
	float mElapsedTime;

	/** 現在メインで動いているシーン */
	IScene* mpActiveScene;
	/** フェード中に表に出てくるシーン */
	IScene* mpStandByScene;

	/** シーンマネージャ内のvoid()系関数ptrの型 */
	using SMfuncType = std::function<void(SceneManager*)> ;

	/** 現在更新する関数 */
	std::vector<SMfuncType> mCurrentUpdates;

	/** 現在描画する関数 */
	std::vector<SMfuncType> mCurrentRenders;

	/** 2Dテクスチャ */
	Microsoft::WRL::ComPtr<ID3D11Texture2D> 
		mTexture2D;
	/** レンダーターゲットビュー */
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> 
		mFadeImageRenderTargetView;
	/** シェーダーリソースビュー */
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> 
		mFadeImageShaderResourceView;

	/** 現在適用されているレンダーターゲットビュー */
	ID3D11RenderTargetView* mpCurrentRTV;
public:

	/**
	 * @brief 初期化関数
	 */
	void Initialize();

	/**
	 * @fn
	 * @brief 更新関数
	 * @param[in] StepTimer
	 */
	void Update(DX::StepTimer const& timer);

	/**
	 * @brief 描画関数
	 */
	void Render();

	/**
	 * @brief シーンを変更する
	 * @param[in] nextScene 次のシーン
	 */
	void ChangeScene(eSceneType nextScene);

private:
	/**
	 * @brief アクティブシーン更新関数
	 */
	void UpdataActiveScene();

	/**
	 * @brief アクティブシーン描画関数
	 */
	void RenderActiveScene();

	/**
	 * @brief スタンバイシーン更新関数
	 */
	void UpdateStandByScene();

	/**
	 * @brief スタンバイシーン描画関数
	 */
	void RenderStandByScene();

private:
	/**
	 * @brief 処理される関数を追加する(active)
	 */
	void AddFuncActive();

	/**
	 * @brief 処理される関数を追加する(stand by)
	 */
	void AddFuncStandBy();

	/**
	 * @brief 処理される関数を除外する(stand by)
	 */
	void RemoveFuncStandBy();

	/**
	 * @brief フェード用Textureなどを設定
	 */
	void InitFadeTextures();

public:
	/**
	 * @brief 直前の1f経過時間を取得
	 * @return 1f経過時間
	 */
	inline float GetElapsedTime() { return mElapsedTime; }

	/**
	 * @brief 現在適用されているレンダーターゲットビューを取得
	 */
	inline ID3D11RenderTargetView* GetCurrentRTV() {
		return mpCurrentRTV;
	}
};