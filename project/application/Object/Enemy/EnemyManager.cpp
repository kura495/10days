#include "EnemyManager.h"
#include "Player/Player.h"
#include "MapObject/FloorManager.h"

void EnemyManager::Init(Player* player, FloorManager* floorManager)
{
	player_ = player;
	floorManager_ = floorManager;

	enemyModel_.push_back(Model::CreateModelFromObj("project/resources/Player", "player.gltf"));
	enemyModel_.push_back(Model::CreateModelFromObj("project/resources/Weapon", "Weapon.obj"));

	// 撃破カウント
	defeatCount_ = 0;

}

void EnemyManager::Update() {

	// 敵キャラ生成
	enemyCreateTimer_ += 1.0f / 60.0f;
	if (enemyCreateTimer_ >= kEnemyCreateInterval_) {

		// 一定数以上の敵キャラがいる場合は生成しない
		if (enemies_.size() >= 30) {
			enemyCreateTimer_ = 0.0f;
			return;
		}

		// マップの障害物のないマスにランダムに生成
		Vector3 pos = floorManager_->GetRandomPosInMap();
		pos.z = 0.0f;


		// 敵キャラ生成
		IEnemy* enemy = new HumanEnemy();
		enemy->Init(pos, enemyModel_, player_, floorManager_);
		enemies_.push_back(enemy);
		enemyCreateTimer_ = 0.0f;

	}


	// 敵キャラ更新
	for (auto it = enemies_.begin(); it != enemies_.end(); ) {
		(*it)->Update();
		

		// 死亡時にリストから削除
		if ((*it)->IsAlive() == false) {
			delete* it;
			it = enemies_.erase(it);

			// 撃破カウントを増やす
			++defeatCount_;
		}
		else {
			++it;
		}
	}

}

void EnemyManager::Draw() {

	// 敵キャラ描画
	for (auto& enemy : enemies_) {
		enemy->Draw();
	}
}
