#include "Floor.h"


void Floor::Init(std::vector<Model*> models, Vector2 pos)
{
	models_ = models;
	world_.Init();
	world_.transform.scale = Vector3(1.0f, 1.0f, 1.0f);
	world_.transform.translate = Vector3(pos.x, pos.y, 0.0f);
	world_.Update();

	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
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
			collider.CollisionDraw();
		}
	}
}

void Floor::OnCollision(const ICollider& Collider)
{
	if (Collider.GetcollitionAttribute() == (Collider::Tag::EnemyAttackFront | Collider::Tag::EnemyAttackSlam)) {

		return;
	}
}