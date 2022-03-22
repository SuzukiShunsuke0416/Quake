#pragma once
#include "Actor.h"
#include "Game/ScreenEffect/BrightChanger.h"
#include "Game/ScreenEffect/AfterGlow.h"

class DebugActor :public Actor {
private:

	BrightChanger* mpBright;
	AfterGlow* mpAfGlow;
	CrossBright* mpCrossBright;

	std::unique_ptr<DirectX::GeometricPrimitive> mGeoPri;

	DirectX::SimpleMath::Matrix mWorld;
public:

	DebugActor();

	~DebugActor();

	void Update() override;

	void Render() override;


};