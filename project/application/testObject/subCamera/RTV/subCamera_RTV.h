#pragma once
#include "PostProsess/renderTextrue/PostProsess.h"

class subCamera_RTV : public PostProsess {
public:
	void addDraw()override;
	void Update()override;
	void CreateBuffer()override;
	void PreDraw()override;
private:
	virtual void CreateRTV(int Index)override;
	virtual void CreateViewport()override;
	virtual void CreateScissor()override;

	Vector2 textureSize_ = {64.0f,64.0f};
};
