/**
 * @file DebugLogManager.cpp
 * @brief 文字を表示する専用のマネージャー
 * @author suzuki.shunsuke
 * @date ???
 */
#include "pch.h"
#include "DebugLogManager.h"
#include <typeinfo>
#include "DeviceResources.h"

using namespace DirectX;

const XMFLOAT2 DebugLogManager::RENDER_START_POS = XMFLOAT2(10, 10);
const float DebugLogManager::RENDER_DISTANCE = 25;
const int DebugLogManager::STRING_LENGTH = 64;

/*====================================================================================
	【初期化】
	・スプライトバッチの設定
	・フォントの設定
====================================================================================*/
void DebugLogManager::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	mSpriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
}

/*====================================================================================
	【描画】
	・この関数が呼ばれるまでに追加された文字たちを順番に描画する
====================================================================================*/
void DebugLogManager::Draw()
{
	int count = 0;
	XMVECTOR color = Colors::Green;

	mSpriteBatch->Begin();

	{
		auto ite = mOutPutLogs.begin();
		for (; ite != mOutPutLogs.end(); ++ite) {
			float renderPosY = RENDER_START_POS.y + RENDER_DISTANCE * count;//一行描画するごとに下にずらして描画させる

			mSpriteFont->DrawString(mSpriteBatch.get(), *ite, DirectX::XMFLOAT2(RENDER_START_POS.x, renderPosY), color);
			count++;
		}
	}
	
	{
		auto ite = mOutPutLogs2.begin();
		for (; ite != mOutPutLogs2.end(); ++ite) {
			float renderPosY = RENDER_START_POS.y + RENDER_DISTANCE * count;//一行描画するごとに下にずらして描画させる

			mSpriteFont->DrawString(mSpriteBatch.get(), *ite, DirectX::XMFLOAT2(RENDER_START_POS.x, renderPosY), color);
			count++;
		}
	}

	{
		auto ite = mOutPutLogsF.begin();
		for (; ite != mOutPutLogsF.end(); ++ite) {
			float renderPosY = RENDER_START_POS.y + RENDER_DISTANCE * count;//一行描画するごとに下にずらして描画させる

			mSpriteFont->DrawString(mSpriteBatch.get(), (*ite).str, DirectX::XMFLOAT2(RENDER_START_POS.x, renderPosY),Colors::OrangeRed);
			count++;
		}
	}
	mSpriteBatch->End();
	
	this->Reset();
}

/*====================================================================================
	【リセット】
	・描画オーダーの入った文字たちのデータを全て消す。
====================================================================================*/
void DebugLogManager::Reset()
{
	for (auto& logs : mOutPutLogs) {
		delete logs;
	}
	mOutPutLogs.clear();

	for (auto& logs : mOutPutLogs2) {
		delete logs;
	}
	mOutPutLogs2.clear();

	for (auto ite = mOutPutLogsF.begin(); ite != mOutPutLogsF.end();) {
		(*ite).timer--;
		if ((*ite).timer < 0) {
			delete (ite->str);
			ite = mOutPutLogsF.erase(ite);
		}
		else {
			++ite;
		}
	}
}

/*====================================================================================
	【ファイナライズ】
	・Frieze描画系のデータも全て消す
====================================================================================*/
void DebugLogManager::Finalize()
{
	for (auto& logs : mOutPutLogs) {
		delete logs;
	}
	mOutPutLogs.clear();

	for (auto& logs : mOutPutLogs2) {
		delete logs;
	}
	mOutPutLogs2.clear();

	for (auto& log : mOutPutLogsF) {
		delete (log.str);
	}
	mOutPutLogsF.clear();
}

