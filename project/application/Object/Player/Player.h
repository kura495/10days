#pragma once
#include "Sprite/Sprite.h"
#include "Model/Model.h"
#include "Input/Input.h"


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

	// 入力
	XINPUT_STATE joyState;

	// スプライト
	std::unique_ptr<Sprite> mainTex_Sprite;
	
	// モデル
	std::unique_ptr<Model> Model;

	// ワールド座標
	WorldTransform mainTex_TF;

};

