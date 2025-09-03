#pragma once
#include "../IEnemy.h"

class Enemy_Bee : public IEnemy {
public:
	Enemy_Bee() {};
	~Enemy_Bee() {};

	virtual void Update() override;
private:

};