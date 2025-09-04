#include "PlayState.h"

void PlayState::Init()
{
	StateNo = GameStateNo::PLAY;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);

	
	

	enemy_Bee = std::make_unique<Enemy_Bee>();
	enemy_Bee->Init("project/resources/BlackTexture.png", { 64.0f,64.0f });


	// プレイヤー 初期化
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Player", "player.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Weapon", "Weapon.obj"));
	player = std::make_unique<Player>();
	player->Init(playerModel_);


	// フォローカメラ 初期化
	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player->GetWorld());
	followCamera->SetOffset(Vector3(0.0f,0.0f,-10.0f));

}

void PlayState::Update()
{
	// フォローカメラ 更新
	followCamera->Update();
	Renderer::SetViewProj(followCamera->GetParameter());

	//enemy_Bee->Update();
	player->Update();
}

void PlayState::Draw()
{
	//enemy_Bee->Draw();

	// 床 描画
	//floor_->Draw();

	// プレイヤー描画
	player->Draw();
}