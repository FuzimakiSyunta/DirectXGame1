#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	/* delete sprite_;*/
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// スプライトの生成
	//textureHandle_ = TextureManager::Load("Tama.png");
	textureHandle_ = TextureManager::Load("Tama.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldtransform_.Initialize();
	//ビュープロジェクションの初期化
	viewprojection_.Initialize();
	//サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("mokugyo.wav");
	//音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewprojection_);

}

void GameScene::Update() 
{
	//スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();
	//座標を{2,1}移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);

	//音声再生
	if (input_->TriggerKey(DIK_SPACE)) {
	//音声停止
		audio_->StopWave(voiceHandle_);
	}
	//デバックテキストの表示
	ImGui::Begin("Debug1");
	//float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	//float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	//ImGui::Text("Kamata Tarow %d.%d.%d", 2050, 12, 31);
	ImGui::End();
	//デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	
	
	//sprite_->Draw();
	
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	model_->Draw(worldtransform_, viewprojection_, textureHandle_);
	
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	// ラインを描画する
	PrimitiveDrawer::GetInstance()->DrawLine3d({10,0, 0}, {0, 0, 0}, {255, 0, 0});
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
