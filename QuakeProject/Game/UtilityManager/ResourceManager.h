/**
 * @file ResourceManager.h
 * @brief ���\�[�X���Ǘ�����}�l�[�W��
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
* @brief ���f���̎�ނ̗񋓑�
*/
enum class ModelType {
	Tank_Base,
};


/**
* @brief ���f���ƃp�X���܂Ƃ߂��\����
*/
struct ModelAndPath {
	/** ���f�� */
	std::unique_ptr<DirectX::Model> model;
	/** �p�X */
	const wchar_t* path;

	/**
	* @brief �R���X�g���N�^
	*/
	ModelAndPath() 
		:model()
		,path()
	{
	}
};


/**
* @brief ���\�[�X���Ǘ�����}�l�[�W��
* @note �S���f�����Ǘ�����B
* @note �S�e�N�X�`�����Ǘ�����B
*/
class ResourceManager :public SingletonFounder<ResourceManager> {
public:
	friend SingletonFounder;
	
private:
	/**
	* @brief �R���X�g���N�^
	*/
	ResourceManager()
		:mModels()
	{

		//�S���f���p�X��ݒ�
		mModels[ModelType::Tank_Base].path = L"Resources/Models/Tank_01Base.cmo";

		//�S���f����ݒ�
		this->AllModelSet();
	};

	/** ���f���̓���z�񁨏��Ăяo�����ɒǉ� */
	std::map<ModelType, ModelAndPath> mModels;

	/** �S�e�N�X�`����ۑ�����z�� */
	std::map<const wchar_t*, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>
		mTextures;
public:

	/**
	 * @brief	��ނ�������āA���f����Ԃ��B
	 * @param[in] type	���f���̃^�C�v
	 * @return ���f���̃|�C���^
	 */
	DirectX::Model* Get(const ModelType& type) {
		return mModels[type].model.get();
	}

	/**
	* @brief �e�N�X�`���̃Q�b�^�[
	* @note �i�Ȃ��ꍇ�͍쐬�j
	*/
	ID3D11ShaderResourceView* GetTexture(const wchar_t* imageName) {
		//�v�f�������ꍇ�͍쐬����
		if (mTextures.find(imageName) == mTextures.end()) {
			//nullptr�ŗv�f�ǉ�
			mTextures[imageName] = nullptr;

			//���������ɐ����t�@�C�������쐬
			std::wstring wstr = L"Resources/Textures/" + std::wstring(imageName);

			//�e�N�X�`������
			auto device = DX::DeviceResources::GetInstance()->GetD3DDevice();
			DirectX::CreateWICTextureFromFile(
				device,
				wstr.c_str(),
				nullptr,
				mTextures[imageName].ReleaseAndGetAddressOf()
			);
		}

		//�e�N�X�`���̃|�C���^��Ԃ�
		return mTextures[imageName].Get();
	}

private:
	/**
	* @brief �S���f���̐����A�z��ɐݒ�
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