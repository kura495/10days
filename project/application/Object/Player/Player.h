#pragma once
#include "Sprite/Sprite.h"

// -- プレイヤークラス -- //
class Player
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mainTexName"> テクスチャ </param>
	/// <param name="texSize"> テクスチャサイズ </param>
	void Init(std::string mainTexName, Vector2 texSize);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

protected:
	std::unique_ptr<Sprite> mainTex_Sprite;
	WorldTransform mainTex_TF;

};

