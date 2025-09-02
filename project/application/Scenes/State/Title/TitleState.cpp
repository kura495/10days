#include"Scenes/State/Title/TitleState.h"

void TitleState::Init()
{
	StateNo = GameStateNo::TITLE;

	Editer::GetInstance()->SetViewProjection(&Renderer::GetViewProjection());
	Editer::GetInstance()->IsEnable(true);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();
	followCamera->GetParameter().translation_ = {0.0f,0.0f,-10.0f};

	titleSprite = std::make_unique<Sprite>();
	titleSprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	titleSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/Title.png");
	title.Init();

	texture = std::make_unique<Sprite>();
	texture->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/TitleTuto.png");
	texture->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
}

void TitleState::Update()
{
	Renderer::SetViewProj(followCamera->GetParameter());

}

void TitleState::Draw()
{

	titleSprite->RendererDraw(title);
	texture->RendererDraw(title);
}
