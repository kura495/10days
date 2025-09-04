#include "Floor.h"


void Floor::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Init();
	world_.Update();

	collider.Init(&world_);
	collider.SetSize({ 15.0f,0.0f,15.0f });
	collider.SetcollitionAttribute(Collider::Tag::Floor);
	collider.SetcollisionMask(~Collider::Tag::Floor);
	collider.OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	collider.colliderName = "Plane";
}

void Floor::Update()
{
	// 座標更新
	world_.Update();
}

void Floor::Draw()
{
	// 描画
	for (Model* model : models_) {
		if (collider.IsUsing) {
			model->RendererDraw(world_);
		}
	}
}

void Floor::OnCollision(const ICollider& Collider)
{
	if (Collider.GetcollitionAttribute() == (Collider::Tag::EnemyAttackFront | Collider::Tag::EnemyAttackSlam)) {

		return;
	}
}