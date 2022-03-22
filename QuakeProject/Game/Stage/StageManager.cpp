/**
 * @file StageManager.cpp
 * @brief ステージ情報を管理するクラス
 * @author suzuki.shunsuke
 * @date 2022/02/03
 */
#include "pch.h"
#include "StageManager.h"
#include "Game/DataBase/ExternalDatabase.h"
#include "Game/ElevationColor/ElevationColorManager.h"
#include "DeviceResources.h"
#include "Game/Collision/SimpleCheckFunctions.h"
#include "StageTriangleManager.h"
#include "Libraries/MyLibraries/PerlinNoise.h"
#include "Game/Camera/CameraManager.h"
#include "Game/DataBase/ExternalDatabase.h"
#include <numeric>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//=====================================================
//		コンストラクタ
//=====================================================
StageManager::StageManager()
	:mpStagePoints()
	,mStageSise()
	,mPointDistance()
	,mMaxHeight()
	,mTopColor()
	,mGroundColor()
	,mpLineBatchDatas()
	,mCenterPosition()
	,mpStageTriangleManager()
	,mStageSidePosBox(0.0f,0.0f,0.0f,0.0f)
	,mpBright()
{
	mpLineBatchDatas = new DataForPrimitiveBatchRender<VertexPositionColorTexture>();
	mpStageTriangleManager = new StageTriangleManager();
	mpBright = std::make_unique<BrightChanger>();
	mpBright->ChangeConstBuffer({ 1.0f,0.25f,0.0008f,0.0f });

}

//=====================================================
//		デストラクタ
//=====================================================
StageManager::~StageManager()
{
	delete mpLineBatchDatas;
	mpLineBatchDatas = nullptr;

	delete mpStageTriangleManager;
	mpStageTriangleManager = nullptr;
}

//=====================================================
//		初期化
//=====================================================
void StageManager::Initialize()
{
	// ステージデータのパラメータを外部ファイルから読み込む
	std::vector<std::string> data;	// 外部ファイルの情報を格納する配列
	int sizeX, sizeY;				// ステージのサイズｘｙ
	auto dataBase = ExternalDatabase::GetInstance();// 外部ファイル読み込みマネージャ
	dataBase->GetDataFromPath(dataBase->GetFilePath(eExPathType::StageData), data);
	// 外部ファイルの順番と揃えて、データを変数らに格納
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[0].c_str(), "%d", &sizeX);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[1].c_str(), "%d", &sizeY);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[2].c_str(), "%f", &mPointDistance);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[3].c_str(), "%f", &mMaxHeight);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[4].c_str(), "%f,%f,%f", &mTopColor.r, &mTopColor.g, &mTopColor.b);
	ExternalDatabase::SettingOwnParamExcludingElementName(
		data[5].c_str(), "%f,%f,%f", &mGroundColor.r, &mGroundColor.g, &mGroundColor.b);


	{// ポイント配列の要素数だけを予め設定
		using namespace std;
		mpStagePoints = vector<vector<StagePoint*>>(sizeY, vector<StagePoint*>(sizeX));
	}

	// ステージポイントの生成
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mpStagePoints[y][x] = new StagePoint();
		}
	}

	// パーリンノイズ生成器を生成
	PerlinNoise NoiseMaker;

	// ステージポイントの初期化
	StagePointInitDesc desc;
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			// 座標
			desc.pos = Vector3(
				(float)(x)*mPointDistance,
				0.0f,
				(float)(y)*mPointDistance
			);

			// 要素番号
			desc.element = { x,y };

			// 隣接点
			int nextX = std::min(x + 1, sizeX - 1);
			int nextY = std::min(y + 1, sizeY - 1);
			desc.nextRight = mpStagePoints[y][nextX];
			desc.nextBottom = mpStagePoints[nextY][x];
			desc.nextRightBottom = mpStagePoints[nextY][nextX];

			// プリミティブバッチ
			desc.pLineBatchDatas = mpLineBatchDatas;

			// 初期化
			mpStagePoints[y][x]->Initialize(desc, NoiseMaker);
		}
	}

	// 今回のステージサイズ
	mStageSise = Int2(sizeX, sizeY);

	// ステージの端座標
	mStageSidePosBox.left = 0.0f;
	mStageSidePosBox.top = 0.0f;
	mStageSidePosBox.right = (sizeX - 1) * mPointDistance;
	mStageSidePosBox.bottom = (sizeY - 1) * mPointDistance;

	// 中央値設定
	mCenterPosition.x = mStageSidePosBox.right / 2.0f;
	mCenterPosition.z = mStageSidePosBox.bottom / 2.0f;

	this->CalculateStageTriangle();
}

//=====================================================
//		更新
//=====================================================
void StageManager::Update()
{
	// ポイント達を更新
	for (const auto& linePoints : mpStagePoints) {
		for (const auto& point : linePoints) {
			point->Update();
		}
	}

	// StageTriangleたちの値設定
	this->CalculateStageTriangle();

	//this->SetPolygonDatas();
}

//=====================================================
//		描画
//=====================================================
void StageManager::Render()
{
	auto& view = CameraManager::GetInstance()->GetViewMatrix();
	auto& proj = CameraManager::GetInstance()->GetProjectionMatrix();
	mpLineBatchDatas->BasicEffectApply(view, proj);


	//TODO 面用シェーダを適用

	for (const auto& line : mpStagePoints) {
		for (const auto& point : line) {
			point->Render();
		}
	}


	/*mpLineBatchDatas->primitiveBatch->Begin();

	//面一括描画
	mpLineBatchDatas->primitiveBatch->DrawIndexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		&mIndices[0],
		mIndices.size(),
		&mVertices[0],
		mVertices.size()
	);

	mpLineBatchDatas->primitiveBatch->End();*/

	mpLineBatchDatas->BasicEffectApply(view, proj);

	mpBright->Begin();// 発光開始

	for (const auto& line : mpStagePoints) {
		for (const auto& point : line) {
			point->LineRender();
		}
	}

	mpBright->End();// 発光終了
}

//=====================================================
//		終了処理
//=====================================================
void StageManager::Finalize()
{
	// ステージポイントを削除
	for (unsigned y = 0; y < mpStagePoints.size(); y++) {
		for (unsigned x = 0; x < mpStagePoints[y].size(); x++) {
			delete mpStagePoints[y][x];
			mpStagePoints[y][x] = nullptr;
		}
	}
}

//=====================================================
//		StageTriangleメンバの追加
//=====================================================
void StageManager::AddTriangleMember(StageTriangle* triangle)
{
	mpStageTriangleManager->AddMember(triangle);
}

//=====================================================
//		StageTriangleメンバの除外
//=====================================================
void StageManager::RemoveTriangleMember(StageTriangle* triangle)
{
	mpStageTriangleManager->RemoveMember(triangle);
}

//=====================================================
//		StageTriangleたちの値設定
//=====================================================
void StageManager::CalculateStageTriangle()
{
	// オーナーに近い点を３つ選び足元の３角形を設定する
	
	int left, right, top, bottom;	// 足元４点のステージポイントの要素番号を格納
	Vector3 ownerPos;				// オーナーの座標（３次元）
	Vector2 ownerPosV2;				// オーナーの座標（XZ面）
	StagePoint* nearPoints[4];		// オーナー付近のステージポイント４点
	StagePoint** points = nullptr;	// ステージトライアングルが持つ３点のポイント配列の先頭アドレス
	// 存在する全てのStageTriangle
	const auto& Triangles = mpStageTriangleManager->GetStageTriangles();

	// 全足元トライアングルの設定を行う
	for (auto triangle : Triangles) {
		// オーナーの座標（３次元、XZ面）の取得
		ownerPos = thf::GetLocation(triangle->GetOwner());
		ownerPosV2 = Vector2(ownerPos.x, ownerPos.z);
		// XZ面座標からステージポイントの要素番号を計算する
		left = (int)(ownerPosV2.x / mPointDistance);
		top = (int)(ownerPosV2.y / mPointDistance);
		right = left + 1;
		bottom = top + 1;

		// 要素番号がステージのサイズを超えていたら処理を飛ばす。
		BoxParam<int> inBox = BoxParam<int>(left, right, top, bottom);
		BoxParam<int> outBox = BoxParam<int>(0, mStageSise.x - 1, 0, mStageSise.y - 1);
		if (SimpleCheckFunctions::IsBoxInBox(outBox, inBox) == false)continue;

		// 付近４つのステージポイントを抜き出す
		nearPoints[0] = mpStagePoints[top][left];
		nearPoints[1] = mpStagePoints[top][right];
		nearPoints[2] = mpStagePoints[bottom][right];
		nearPoints[3] = mpStagePoints[bottom][left];

		// 四角形の対角線を形成する２点を除いた残りの２点で近い方を使用し３角形を生成する。
		if ((thf::GetLocationFromSky(nearPoints[1]) - ownerPosV2).Length() >
			((thf::GetLocationFromSky(nearPoints[3]) - ownerPosV2).Length())) 
		{
			nearPoints[1] = nearPoints[2];
			nearPoints[2] = nearPoints[3];
		}

		// 三角形の３点のステージポイントを書き換える
		points = triangle->GetStagePoints();
		for (int i = 0; i < StageTriangle::POINT_SIZE; i++) {
			points[i] = nearPoints[i];
		}

		// その他StageTriangleの更新処理を行う
		triangle->Update();
	}
}

//=====================================================
//		面描画時に使う値たちの設定
//=====================================================
void StageManager::SetPolygonDatas()
{
	// ステージポイントの総数
	int serialLength = mStageSise.x * mStageSise.y;

	//============= 頂点情報 =============

	// 一旦配列を空に（キャパは先に確保）
	mVertices.clear();
	mVertices.reserve(serialLength);

	// 減色率を設定
	Vector4 subtractiveColorRat;
	{
		float r = 0.5f;
		subtractiveColorRat = Vector4(r, r, r, 1.0f);
	}
	
	// 頂点情報を設定（単純に配列の順番で）
	for (const auto& linePoints : mpStagePoints) {
		for (const auto& point : linePoints) {
			auto vertex = VertexPositionColorTexture(
				point->GetTransform().Location,
				point->GetElevationColor()->GetColor().V4() * subtractiveColorRat,
				//Colors::Red,
				Vector2::Zero
			);

			mVertices.push_back(vertex);
		}
	}

	//============= 順番情報 =============
	
	// 一旦配列を空に
	mIndices.clear();

	// 現在追加設定している要素番号
	Int2 indexI2 = Int2(0,-1);
	
	// 頂点情報にアクセスする際に使用されるインデックスの値を一つなぎの配列として考え、設定する際に使用する。
	int* serialNums = new int[serialLength + 1];
	std::iota(serialNums, serialNums + serialLength + 1, 0);//自動連番設定関数


	//現在左右のどちらに進んでいるか
	const unsigned moveLeft =	(1 << 0);
	const unsigned moveRight =	(1 << 1);
	const unsigned moveUp =		(1 << 2);
	const unsigned moveDown =	(1 << 3);

	unsigned moveWay = moveUp;

	//moveWayの値に応じてindexI2の値を設定する関数
	auto SetIndexWithWay = [&]() {
		if (moveWay & moveLeft)	{ indexI2.x--; }
		if (moveWay & moveRight){ indexI2.x++; }
		if (moveWay & moveUp)	{ indexI2.y--; }
		if (moveWay & moveDown)	{ indexI2.y++; }
	};

	//現在の移動方向を元に次の移動方向を設定する関数
	//※上の次は右下、右下の次は上
	//※下の次は左上、左上の次は下
	auto SetWaywithWay = [&]() {
		if		(moveWay == (moveUp))
			{ moveWay = (moveRight | moveDown); }

		else if (moveWay == (moveRight | moveDown))
			{ moveWay = moveUp; }
		
		else if (moveWay == (moveDown))
			{ moveWay = (moveLeft | moveUp); }
		
		else if (moveWay == (moveLeft | moveUp))
			{ moveWay = moveDown; }
	};

	//indexI2の値に応じて連番値を返す関数
	auto SetInediceWithSerial = [&]() {
		if (indexI2.y >= mStageSise.y) return true;

		if (indexI2.y < 0)return false;

		mIndices.push_back(serialNums[indexI2.x + indexI2.y * mStageSise.x]);
		return false;
	};

	//全ての値を設定するまで
	while (true) 
	{
		
		// 左端に来ている場合
		if (indexI2.x == 0) {
			SetInediceWithSerial();

			//現在の下の値を追加
			indexI2.y++;
			SetInediceWithSerial();

			//更にもう１つ下の値を追加
			indexI2.y++;
			bool isOver = SetInediceWithSerial();
			if (isOver)break;

			//右に一つずらしておく
			indexI2.y--;
			SetInediceWithSerial();

			//次が「上」になるように、一時的に「右下」と設定
			moveWay = (moveRight | moveDown);

			indexI2.x++;
			indexI2.y++;
		}
		// 右端に来ている場合
		else if (indexI2.x == mStageSise.x - 1) {
			//まず現在の上の値を追加
			//indexI2.y--;
			SetInediceWithSerial();

			//その後その下（元いた場所）の値を追加
			indexI2.y--;
			SetInediceWithSerial();

			//さらにその下の値を追加
			indexI2.y++;
			indexI2.y++;
			bool isOver = SetInediceWithSerial();
			if (isOver)break;

			// 左上にずらしておく
			indexI2.x--;
			indexI2.y--;

			//次が
			moveWay = (moveLeft | moveUp);
		}

		//現在の値を追加
		SetInediceWithSerial();

		//移動方向フラグの設定
		SetWaywithWay();
		//次の場所へずらす
		SetIndexWithWay();

	}

	delete[] serialNums;
}
