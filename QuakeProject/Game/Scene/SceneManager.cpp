/**
 * @file SceneManager.cpp
 * @brief 各シーンを管理するクラス
 * @author suzuki.shunsuke
 * @date 2022/01/29
 */
#include "pch.h"
#include "SceneManager.h"

#include "TitleScene.h"
#include "PlayScene.h"
#include "DeviceResources.h"
#include "Game/UtilityManager/InputManager.h"
#include "Game/DataBase/ExternalDatabase.h"
#include "Game.h"

const int SceneManager::MAX_FUNCTION_COUNT = 2;

//=====================================================
//		コンストラクタ
//=====================================================
SceneManager::SceneManager()
	:mElapsedTime(0.0f)
	,mpActiveScene(nullptr)
	,mpStandByScene(nullptr)
	,mCurrentUpdates{}
	,mCurrentRenders{}
	,mpCurrentRTV(nullptr)
{
	// 現在RTVを仮設定
	mpCurrentRTV = DX::DeviceResources::GetInstance()->GetRenderTargetView();

	// 外部ファイル読み込み
	ExternalDatabase::GetInstance()->Initialize();

	// サウンドマネージャの初期化
	SoundManager::GetInstance()->Initialize();
}

//=====================================================
//		デストラクタ
//=====================================================
SceneManager::~SceneManager()
{
	// アクティブシーンが残っていた場合は削除
	if (mpActiveScene) {
		mpActiveScene->Finalize();
		delete mpActiveScene;
		mpActiveScene = nullptr;
	}
	// スタンバイシーンが残っていた場合は削除
	if (mpStandByScene) {
		mpStandByScene->Finalize();
		delete mpStandByScene;
		mpStandByScene = nullptr;
	}

	// (全シーンで使われる)各マネージャ達の解放
	DebugObjManager::GetInstance()->SingletonKill();
	DebugLogManager::GetInstance()->SingletonKill();
	DebugImgManager::GetInstance()->SingletonKill();
	InputManager::GetInstance()->SingletonKill();
	SoundManager::GetInstance()->SingletonKill();
	ExternalDatabase::GetInstance()->SingletonKill();
}

//=====================================================
//		初期化
//=====================================================
void SceneManager::Initialize()
{
	// ゲーム起動時にあるシーンを生成
	mpActiveScene = new TitleScene();
	
	//通常処理を追加
	this->AddFuncActive();

	//フェード用Textureなどを設定
	this->InitFadeTextures();
}

//=====================================================
//		更新
//=====================================================
void SceneManager::Update(DX::StepTimer const& timer)
{
	mElapsedTime = float(timer.GetElapsedSeconds());

	// 入力系マネージャの更新
	InputManager::GetInstance()->Update();

	// サウンドマネージャの更新
	SoundManager::GetInstance()->Update();

	// デバッグログへfpsを登録
	DebugLogManager::GetInstance()->EntryLog((int)(timer.GetFramesPerSecond()), L"fps");

	// 現在追加されている更新処理を一括で呼ぶ
	for (auto& update : mCurrentUpdates) {
		update(this);
	}

	// escキーが押されたら、ゲームを終了する
	auto& keyboard = InputManager::GetInstance()->GetKeyboardTracker();
	if (keyboard.IsKeyReleased(DirectX::Keyboard::Escape)) {
		PostQuitMessage(0);
	}
}

//=====================================================
//		描画
//=====================================================
void SceneManager::Render()
{
	// 現在追加されている描画処理を一括で呼ぶ
	for (auto& render : mCurrentRenders) {
		render(this);
	}

	//デバッグ表示の描画
	DebugImgManager::GetInstance()->Draw();
	DebugLogManager::GetInstance()->Draw();
}

//=====================================================
//		シーン変更
//=====================================================
void SceneManager::ChangeScene(eSceneType nextScene)
{
	// 引数に指定されたシーンを生成する
	switch (nextScene)
	{
		case eSceneType::Title:

			//すでにスタンバイシーンが生成されていたら無視する
			if (mpStandByScene)return;
			//タイトルシーンの生成
			mpStandByScene = new TitleScene();
			mpStandByScene->Initialize();

			break;

		case eSceneType::Play:

			//すでにスタンバイシーンが生成されていたら無視する
			if (mpStandByScene)return;
			//プレイシーンの生成
			mpStandByScene = new PlayScene();
			mpStandByScene->Initialize();

			break;

		default:
			assert(!"ChangeScene　対応外タイプが侵入");
			break;
	}

	// スタンバイ状態のシーンも処理を開始させる
	this->AddFuncStandBy();

	// フェードが開始されることをactive,standby共に伝える
	mpActiveScene->OnFadeStart();
	mpStandByScene->OnFadeStart();
}

//=====================================================
//		アクティブシーン更新
//=====================================================
void SceneManager::UpdataActiveScene()
{
	mpActiveScene->Update();
}

//=====================================================
//		アクティブシーン描画
//=====================================================
void SceneManager::RenderActiveScene()
{
	mpActiveScene->Render();
}

//=====================================================
//		スタンバイシーン更新
//=====================================================
void SceneManager::UpdateStandByScene()
{
	mpStandByScene->Update();
	bool endFlag = mpStandByScene->FadeUpdate();

	// フェードが完了した場合
	if (endFlag == true) {
		// スタンバイ系関数を配列から抜く
		this->RemoveFuncStandBy();

		// フェードが終わったことを伝える
		mpActiveScene->OnFadeEnd();
		mpStandByScene->OnFadeEnd();

		// フェードが終わり全てが表示された
		// シーンをアクティブシーンとして扱う。
		std::swap(mpActiveScene, mpStandByScene);

		// 前アクティブシーンを削除する
		mpStandByScene->Finalize();
		delete mpStandByScene;
		mpStandByScene = nullptr;
	}
}

//=====================================================
//		スタンバイシーン描画
//=====================================================
void SceneManager::RenderStandByScene()
{
	// デバイスリソースを取得
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	auto renderTarget = pDR->GetRenderTargetView();
	auto depthStencil = pDR->GetDepthStencilView();

	// 用意したテクスチャにレンダリング
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		mFadeImageRenderTargetView.GetAddressOf(),
		depthStencil
	);
	mpCurrentRTV = mFadeImageRenderTargetView.Get();

	// スタンバイシーン描画（退避用rtvに）
	mpStandByScene->Render();

	// レンダーターゲットを元に戻す
	pDR->GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		&renderTarget,
		depthStencil
	);
	mpCurrentRTV = renderTarget;

	// フェードを適用させて描画（画面に）
	mpStandByScene->FadeRender(mFadeImageShaderResourceView.Get());

	// 自前レンダーターゲットビューをクリア
	context->ClearRenderTargetView(
		mFadeImageRenderTargetView.Get(),
		ConstDataBase::BackGroundColor
	);
}


//=====================================================
//		処理関数追加(active)
//=====================================================
void SceneManager::AddFuncActive()
{
	if (mCurrentUpdates.size() != 0)assert(!"更新関数が余分に追加されています");
	if (mCurrentRenders.size() != 0)assert(!"描画関数が余分に追加されています");

	mCurrentUpdates.push_back(
		std::mem_fn(&SceneManager::UpdataActiveScene));
	mCurrentRenders.push_back(
		std::mem_fn(&SceneManager::RenderActiveScene));
}

//=====================================================
//		処理関数追加(stand by)
//=====================================================
void SceneManager::AddFuncStandBy()
{
	if (mpStandByScene == nullptr) {
		assert(!"mpStandBySceneにシーンが設定されていません");
	}

	if (mCurrentUpdates.size() >= MAX_FUNCTION_COUNT)assert(!"更新関数が余分に追加されています");
	if (mCurrentRenders.size() >= MAX_FUNCTION_COUNT)assert(!"描画関数が余分に追加されています");

	mCurrentUpdates.push_back(
		std::mem_fn(&SceneManager::UpdateStandByScene));
	mCurrentRenders.push_back(
		std::mem_fn(&SceneManager::RenderStandByScene));
}

//=====================================================
//		処理関数除外(stand by)
//=====================================================
void SceneManager::RemoveFuncStandBy()
{
	// スタンバイ更新関数を抜く
	if (mCurrentUpdates.size() == MAX_FUNCTION_COUNT) {
		mCurrentUpdates.pop_back();
	}

	// スタンバイ描画関数を抜く
	if (mCurrentRenders.size() == MAX_FUNCTION_COUNT) {
		mCurrentRenders.pop_back();
	}
}

//=====================================================
//		フェード用Textureなどを設定
//=====================================================
void SceneManager::InitFadeTextures()
{
	// デバイスリソースを取得
	auto pDR = DX::DeviceResources::GetInstance();

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
	pDR->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, mTexture2D.GetAddressOf());

	// レンダーターゲットビューの生成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	pDR->GetD3DDevice()->CreateRenderTargetView(mTexture2D.Get(), &rtvDesc, mFadeImageRenderTargetView.GetAddressOf());

	// シェーダーリソースビューの生成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	pDR->GetD3DDevice()->CreateShaderResourceView(mTexture2D.Get(), &srvDesc, mFadeImageShaderResourceView.GetAddressOf());
}


/* 
	～シーン遷移の流れ～

	①シーン変更命令がこのクラスに届く。
	②次シーンを作成する。
	③前シーンの描画の上に、フェード処理がかかった次シーンが描画される
	④フェードが終わったら前シーンが削除され、次シーンが単体処理されるようになる。
*/