#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<map>
#include<unordered_map>
#include<DirectXTex.h>
#include<wrl.h>
#include<string>
#include<functional>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

namespace KochaEngine
{
	class Dx12_Wrapper
	{
		SIZE _winSize;
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		//DXGIまわり
		ComPtr < IDXGIFactory6> dxgiFactory = nullptr;//DXGIインターフェイス
		ComPtr < IDXGISwapChain4> _swapchain = nullptr;//スワップチェイン

		//DirectX12まわり
		ComPtr< ID3D12Device> _dev = nullptr;//デバイス
		ComPtr < ID3D12CommandAllocator> _cmdAllocator = nullptr;//コマンドアロケータ
		ComPtr < ID3D12GraphicsCommandList> _cmdList = nullptr;//コマンドリスト
		ComPtr < ID3D12CommandQueue> _cmdQueue = nullptr;//コマンドキュー

		//表示に関わるバッファ周り
		ComPtr<ID3D12Resource> _depthBuffer = nullptr;//深度バッファ
		std::vector<ComPtr<ID3D12Resource>> _backBuffers;//バックバッファ(2つ以上…スワップチェインが確保)
		ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr;//レンダーターゲット用デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;//深度バッファビュー用デスクリプタヒープ
		D3D12_VIEWPORT _viewport;//ビューポート
		D3D12_RECT _scissorrect;//シザー矩形

		//シーンを構成するバッファまわり
		ComPtr<ID3D12Resource> _sceneConstBuff = nullptr;

		struct SceneData {
			DirectX::XMMATRIX view;//ビュー行列
			DirectX::XMMATRIX proj;//プロジェクション行列
			DirectX::XMFLOAT3 eye;//視点座標
		};
		SceneData* _mappedSceneData;
		ComPtr<ID3D12DescriptorHeap> _sceneDescHeap = nullptr;

		//フェンス
		ComPtr<ID3D12Fence> _fence = nullptr;
		UINT64 _fenceVal = 0;

		ComPtr<ID3D12DescriptorHeap> _heapForImgui;

		//マルチパスレンダリング用
		ComPtr<ID3D12Resource>_peraResource;
		ComPtr<ID3D12DescriptorHeap> _peraRTVHeap; //レンダーターゲット
		ComPtr<ID3D12DescriptorHeap> _peraSRVHeap; //テクスチャ用

		//最終的なレンダーターゲットの生成
		HRESULT	CreateFinalRenderTargets();
		//デプスステンシルビューの生成
		HRESULT CreateDepthStencilView();

		//スワップチェインの生成
		HRESULT CreateSwapChain(const HWND& arg_hwnd);

		//DXGIまわり初期化
		HRESULT InitializeDXGIDevice();

		//コマンドまわり初期化
		HRESULT InitializeCommand();

		//ビュープロジェクション用ビューの生成
		HRESULT CreateSceneView();

		ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui();

		HRESULT CreatePeraResource();

	public:
		Dx12_Wrapper(const HWND& arg_hwnd);
		~Dx12_Wrapper();

		void Update();
		void BeginDraw(float r, float g, float b);
		void EndDraw();
		///テクスチャパスから必要なテクスチャバッファへのポインタを返す
		///@param texpath テクスチャファイルパス
		ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);

		ComPtr < ID3D12Device> GetDevice();//デバイス
		ComPtr < ID3D12GraphicsCommandList> GetCmdList();//コマンドリスト
		ComPtr < IDXGISwapChain4> GetSwapchain();//スワップチェイン
		ComPtr < ID3D12CommandQueue> GetQue();
		ComPtr < ID3D12DescriptorHeap> GetHeapForImgui();
		SIZE GetWinSize();

		void SetScene();

	};
}