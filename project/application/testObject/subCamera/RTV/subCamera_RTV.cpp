#include "subCamera_RTV.h"

void subCamera_RTV::addDraw()
{
	
}

void subCamera_RTV::Update()
{

}

void subCamera_RTV::CreateBuffer()
{

}

void subCamera_RTV::PreDraw()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	DirectX->GetcommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	//指定した色で画面全体をクリア
	float clearColor[] = { 0.0f,1.0f,0.0f,1.0f };//いつもの青っぽいやつ
	DirectX->GetcommandList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	//指定した深度で画面全体をクリアする
	DirectX->GetcommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { DirectX->GetsrvDescriptorHeap().Get() };
	DirectX->GetcommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	DirectX->GetcommandList()->RSSetViewports(1, &viewport);
	DirectX->GetcommandList()->RSSetScissorRects(1, &scissorRect);
}

void subCamera_RTV::CreateRTV(int Index)
{
	//RTVを作る
	const Vector4 kRenderTargetClearValue{ 0.0f,1.0f,0.0f,1.0f };
	renderTextureResource = CreateRenderTextureResource(DirectX->GetDevice(), (uint32_t)textureSize_.x, (uint32_t)textureSize_.y, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	uint32_t descriptorSizeRTV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	rtvHandle.ptr = DirectX->GetrtvHandles().ptr;
	for (int i = 0; i < Index; i++) {
		rtvHandle.ptr += descriptorSizeRTV;
	}
	//DirectXCommon::MakeDescriptorHeapの//rtvディスクリプタヒープの作成のNumDescriptorsを変える
	DirectX->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &rtvDesc, rtvHandle);
}

void subCamera_RTV::CreateViewport()
{
	viewport.Width = (float)textureSize_.x;
	viewport.Height = (float)textureSize_.y;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void subCamera_RTV::CreateScissor()
{
	scissorRect.left = 0;
	scissorRect.right = (uint32_t)textureSize_.x;
	scissorRect.top = 0;
	scissorRect.bottom = (uint32_t)textureSize_.y;
}
