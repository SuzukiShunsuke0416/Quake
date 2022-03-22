#include "pch.h"
#include "DebugActor.h"
#include "DeviceResources.h"
#include "Game/Camera/CameraManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

DebugActor::DebugActor()
{
	mpBright = new BrightChanger();
	mpBright->ChangeConstBuffer({0.0f,0.3f,0.0025f,0.0f});
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto context = pDR->GetD3DDeviceContext();
	mGeoPri = GeometricPrimitive::CreateSphere(context);
	mWorld = Matrix::Identity;

	mpAfGlow = new AfterGlow(0.9f, 32);
	mpCrossBright = new CrossBright();
}

DebugActor::~DebugActor()
{
	delete mpBright;
	delete mpAfGlow;
	delete mpCrossBright;
}

void DebugActor::Update()
{
	static float ang = 0.0f;
	ang += 0.1f;
	mWorld = Matrix::CreateTranslation(Vector3(5, 0, 0));
	mWorld *= Matrix::CreateRotationZ(ang * 0.1f);
	mWorld *= Matrix::CreateRotationY(ang);

	Plane p;
	Ray r;
}

void DebugActor::Render()
{
	const auto& view = CameraManager::GetInstance()->GetViewMatrix();
	const auto& proj = CameraManager::GetInstance()->GetProjectionMatrix();

	Matrix posMat = Matrix::CreateTranslation(Vector3(0.0f,20.0f,0.0f));

	mGeoPri->Draw(
		Matrix::CreateScale(5.0f) * posMat,
		view,
		proj,
		Colors::Purple
	);

	mpBright->Begin();// ”­ŒõŠJŽn

	mGeoPri->Draw(
		Matrix::CreateScale(5.0f) * posMat,
		view,
		proj,
		Colors::Purple
	);

	mpBright->End();// ”­ŒõI—¹

	

	/*mpBright->Begin();

	mGeoPri->Draw(
		mWorld,
		view,
		proj,
		Colors::Purple
	);

	mpBright->End();*/

	mpAfGlow->Adaptation(mGeoPri.get(), mWorld * posMat, Colors::YellowGreen);
}
