#include "PlayState.h"

void PlayState::Init()
{
	StateNo = GameStateNo::PLAY;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);

	//コリジョンマネージャー
	collisionManager = std::make_unique<CollisionManager>();
	collisionManager->Init();

	// 床 初期化
	floorManager_ = std::make_unique<FloorManager>();
	floorManager_->Init();

	// プレイヤー 初期化
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Player", "player.gltf"));
	playerModel_.push_back(Model::CreateModelFromObj("project/resources/Weapon", "Weapon.obj"));
	player = std::make_unique<Player>();
	player->Init(playerModel_);

	
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init(player.get(), floorManager_.get());

	// フォローカメラ 初期化
	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->SetTarget(&player->GetWorld());
	followCamera->SetOffset(Vector3(0.0f,0.0f,-30.0f));

	// 背景(仮) 初期化
	skydome_ = Model::CreateModelFromObj("project/resources/SkyDome", "SkyDome.obj");

	backgroundWorld_.Init();
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Init({ 0.5f,0.5f },Vector2(1920,1080));
	backgroundSprite_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/PlayStateBack.png");

}

void PlayState::Update()
{

	// フォローカメラ 更新
	followCamera->Update();
	Renderer::SetViewProj(followCamera->GetParameter());

	backgroundWorld_.Update();
	player->Update();
	enemyManager_->Update();
	floorManager_->Update();

	// 撃破カウント
	int32_t count = enemyManager_->GetDefeatCount();
	if (count > 20) {
		// クリア
		if (StateNo != GameStateNo::CLEAR) {
			StateNo = GameStateNo::CLEAR;
		}
	}
	

	// コリジョンマネージャー 更新
	collisionManager->Update();

}

void PlayState::Draw()
{
	skydome_->RendererDraw(backgroundWorld_);

	//backgroundSprite_->RendererDraw(backgroundWorld_);

	// プレイヤー描画
	player->Draw();
	
	// 敵キャラ 描画
	enemyManager_->Draw();


	// 床 描画
	floorManager_->Draw();
	
}