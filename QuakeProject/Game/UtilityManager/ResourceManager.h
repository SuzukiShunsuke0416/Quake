/**
 * @file ResourceManager.h
 * @brief リソースを管理するマネージャ
 * @author suzuki.shunsuke
 */
#pragma once
#include "SingletonFounder.h"
#include <Model.h>
#include <map>
#include <string>
#include <xstring>
#include "DeviceResources.h"
#include <WICTextureLoader.h>
#include <Effects.h>


/**
* @brief モデルの種類の列挙体
*/
enum class ModelType {
	Tank_Base,
};


/**
* @brief モデルとパスをまとめた構造体
*/
struct ModelAndPath {
	/** モデル */
	std::unique_ptr<DirectX::Model> model;
	/** パス */
	const wchar_t* path;

	/**
	* @brief コンストラクタ
	*/
	ModelAndPath() 
		:model()
		,path()
	{
	}
};


/**
* @brief リソースを管理するマネージャ
* @note 全モデルを管理する。
* @note 全テクスチャを管理する。
*/
class ResourceManager :public SingletonFounder<ResourceManager> {
public:
	friend SingletonFounder;
	
private:
	/**
	* @brief コンストラクタ
	*/
	ResourceManager()
		:mModels()
	{

		//全モデルパスを設定
		mModels[ModelType::Tank_Base].path = L"Resources/Models/Tank_01Base.cmo";

		//全モデルを設定
		this->AllModelSet();
	};

	/** モデルの入る配列→初呼び出し時に追加 */
	std::map<ModelType, ModelAndPath> mModels;

	/** 全テクスチャを保存する配列 */
	std::map<const wchar_t*, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>
		mTextures;
public:

	/**
	 * @brief	種類をもらって、モデルを返す。
	 * @param[in] type	モデルのタイプ
	 * @return モデルのポインタ
	 */
	DirectX::Model* Get(const ModelType& type) {
		return mModels[type].model.get();
	}

	/**
	* @brief テクスチャのゲッター
	* @note （ない場合は作成）
	*/
	ID3D11ShaderResourceView* GetTexture(const wchar_t* imageName) {
		//要素が無い場合は作成する
		if (mTextures.find(imageName) == mTextures.end()) {
			//nullptrで要素追加
			mTextures[imageName] = nullptr;

			//引数を元に正式ファイル名を作成
			std::wstring wstr = L"Resources/Textures/" + std::wstring(imageName);

			//テクスチャ生成
			auto device = DX::DeviceResources::GetInstance()->GetD3DDevice();
			DirectX::CreateWICTextureFromFile(
				device,
				wstr.c_str(),
				nullptr,
				mTextures[imageName].ReleaseAndGetAddressOf()
			);
		}

		//テクスチャのポインタを返す
		return mTextures[imageName].Get();
	}

private:
	/**
	* @brief 全モデルの生成、配列に設定
	*/
	void AllModelSet() {
		auto device = DX::DeviceResources::GetInstance()->GetD3DDevice();
		
		for (auto ite = mModels.begin(); ite != mModels.end(); ++ite) {

			std::unique_ptr<DirectX::EffectFactory> factory = std::make_unique<DirectX::EffectFactory>(device);
			factory->SetDirectory(L"Resources/Models");

			ite->second.model = DirectX::Model::CreateFromCMO(
				device,
				ite->second.path,
				*factory
			);
		}
	}
};