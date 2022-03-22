/**
 * @file SceneChangeFader.cpp
 * @brief �V�[���؂�ւ����̃t�F�[�h��S������N���X
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

// �X�V�֐�(�萔)��ݒ�
const std::function<bool(SceneChangeFader*)> SceneChangeFader::msUpdateFunctions[] =
{ 
	std::mem_fn(&SceneChangeFader::NormalUpdate),//�ʏ�X�V�֐� (Normal)
};

// �萔�l�ݒ�
const float SceneChangeFader::NORMAL_PARAM_END = 1.0f;

//=====================================================
//		�R���X�g���N�^
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
//		������
//=====================================================
void SceneChangeFader::Initialize(eSceneChangeFadeType type)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	// �v���~�e�B�u�o�b�`�̍쐬
	mPrimitiveBatch = std::make_unique<
		PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// cso�܂ł̃p�X���ꊇ�Ǘ�
	std::map<eSceneChangeFadeType, wchar_t*> shaderPathsVS;
	std::map<eSceneChangeFadeType, wchar_t*> shaderPathsPS;
	shaderPathsVS[eSceneChangeFadeType::Normal] = L"Resources/Shaders/ParticleVS.cso";
	shaderPathsPS[eSceneChangeFadeType::Normal] = L"Resources/Shaders/ParticlePS.cso";

	// cso�t�@�C���^�C�v�ɉ����Ă�ǂݎ��
	BinaryFile VSData = BinaryFile::LoadFile(shaderPathsVS[type]);
	BinaryFile PSData = BinaryFile::LoadFile(shaderPathsPS[type]);

	// ���̓��C�A�E�g��ݒ�
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		mInputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL,
		mVertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		assert(L"CreateVertexShader Failed.");
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL,
		mPixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		assert(!L"CreatePixelShader Failed.");
	}

	// �萔�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &mCBuffer);

	// �R�����X�e�[�g�ݒ�
	mStates = std::make_unique<CommonStates>(device);

	// �X�V�֐��ݒ�
	mUpdate = msUpdateFunctions[(unsigned int)(type)];
}

//=====================================================
//		�X�V
//=====================================================
bool SceneChangeFader::Update()
{
	return mUpdate(this);
}

//=====================================================
//		�`��
//=====================================================
void SceneChangeFader::Draw(ID3D11ShaderResourceView* srv)
{
	auto pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();

	//�V�F�[�_�ɓn���p�����[�^�̐ݒ�
	ConstBuffer cbuff;
	cbuff.matView = Matrix::Identity.Transpose();
	cbuff.matProj = Matrix::Identity.Transpose();
	cbuff.matWorld = Matrix::Identity.Transpose();
	cbuff.Time = mTimeParam;

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(mCBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = mStates->NonPremultiplied();
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�͎Q�Ƃ̂�
	context->OMSetDepthStencilState(mStates->DepthRead(), 0);
	// �J�����O
	context->RSSetState(mStates->CullNone());

	//�萔�o�b�t�@���V�F�[�_�ɓn��
	ID3D11Buffer* cb[1] = { mCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);


	//�T���v���[�A�V�F�[�_�A�摜�����ꂼ��o�^
	ID3D11SamplerState* sampler[1] = { mStates->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, &srv);

	//���̓��C�A�E�g�𔽉f
	context->IASetInputLayout(mInputLayout.Get());

	// ���O����������̂ňꎞ�I�ɏȗ�
	using VertexPCT = VertexPositionColorTexture;

	VertexPCT ver[] = {
		VertexPCT(Vector3(-1.0f,1.0f,0.0f),Vector4::Zero,Vector2(0.0f,0.0f)),
		VertexPCT(Vector3(1.0f,1.0f,0.0f),Vector4::Zero,Vector2(1.0f,0.0f)),
		VertexPCT(Vector3(1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(1.0f,1.0f)),
		VertexPCT(Vector3(-1.0f,-1.0f,0.0f),Vector4::Zero,Vector2(0.0f,1.0f))
	};

	//�o�b�`�ɒ��_����n��
	mPrimitiveBatch->Begin();
	mPrimitiveBatch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	mPrimitiveBatch->End();


	//���̃��f���ɉe�����o��\��������̂Ŏg���I�������V�F�[�_���O��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

//=====================================================
//		�X�V�y�ʏ�z
//=====================================================
bool SceneChangeFader::NormalUpdate()
{
	mTimeParam.x += SceneManager::GetInstance()->GetElapsedTime();

	DebugLogManager::GetInstance()->EntryLog(mTimeParam.x,L"timeVal");

	return mTimeParam.x >= NORMAL_PARAM_END;
}
