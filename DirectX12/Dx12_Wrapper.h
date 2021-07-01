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

		//DXGI�܂��
		ComPtr < IDXGIFactory6> dxgiFactory = nullptr;//DXGI�C���^�[�t�F�C�X
		ComPtr < IDXGISwapChain4> _swapchain = nullptr;//�X���b�v�`�F�C��

		//DirectX12�܂��
		ComPtr< ID3D12Device> _dev = nullptr;//�f�o�C�X
		ComPtr < ID3D12CommandAllocator> _cmdAllocator = nullptr;//�R�}���h�A���P�[�^
		ComPtr < ID3D12GraphicsCommandList> _cmdList = nullptr;//�R�}���h���X�g
		ComPtr < ID3D12CommandQueue> _cmdQueue = nullptr;//�R�}���h�L���[

		//�\���Ɋւ��o�b�t�@����
		ComPtr<ID3D12Resource> _depthBuffer = nullptr;//�[�x�o�b�t�@
		std::vector<ComPtr<ID3D12Resource>> _backBuffers;//�o�b�N�o�b�t�@(2�ȏ�c�X���b�v�`�F�C�����m��)
		ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr;//�����_�[�^�[�Q�b�g�p�f�X�N���v�^�q�[�v
		ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;//�[�x�o�b�t�@�r���[�p�f�X�N���v�^�q�[�v
		D3D12_VIEWPORT _viewport;//�r���[�|�[�g
		D3D12_RECT _scissorrect;//�V�U�[��`

		//�V�[�����\������o�b�t�@�܂��
		ComPtr<ID3D12Resource> _sceneConstBuff = nullptr;

		struct SceneData {
			DirectX::XMMATRIX view;//�r���[�s��
			DirectX::XMMATRIX proj;//�v���W�F�N�V�����s��
			DirectX::XMFLOAT3 eye;//���_���W
		};
		SceneData* _mappedSceneData;
		ComPtr<ID3D12DescriptorHeap> _sceneDescHeap = nullptr;

		//�t�F���X
		ComPtr<ID3D12Fence> _fence = nullptr;
		UINT64 _fenceVal = 0;

		ComPtr<ID3D12DescriptorHeap> _heapForImgui;

		//�}���`�p�X�����_�����O�p
		ComPtr<ID3D12Resource>_peraResource;
		ComPtr<ID3D12DescriptorHeap> _peraRTVHeap; //�����_�[�^�[�Q�b�g
		ComPtr<ID3D12DescriptorHeap> _peraSRVHeap; //�e�N�X�`���p

		//�ŏI�I�ȃ����_�[�^�[�Q�b�g�̐���
		HRESULT	CreateFinalRenderTargets();
		//�f�v�X�X�e���V���r���[�̐���
		HRESULT CreateDepthStencilView();

		//�X���b�v�`�F�C���̐���
		HRESULT CreateSwapChain(const HWND& hwnd);

		//DXGI�܂�菉����
		HRESULT InitializeDXGIDevice();

		//�R�}���h�܂�菉����
		HRESULT InitializeCommand();

		//�r���[�v���W�F�N�V�����p�r���[�̐���
		HRESULT CreateSceneView();

		ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui();

		HRESULT CreatePeraResource();

	public:
		Dx12_Wrapper(HWND hwnd);
		~Dx12_Wrapper();

		void Update();
		void BeginDraw(float r, float g, float b);
		void EndDraw();
		///�e�N�X�`���p�X����K�v�ȃe�N�X�`���o�b�t�@�ւ̃|�C���^��Ԃ�
		///@param texpath �e�N�X�`���t�@�C���p�X
		ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);

		ComPtr < ID3D12Device> GetDevice();//�f�o�C�X
		ComPtr < ID3D12GraphicsCommandList> GetCmdList();//�R�}���h���X�g
		ComPtr < IDXGISwapChain4> GetSwapchain();//�X���b�v�`�F�C��
		ComPtr < ID3D12DescriptorHeap> GetHeapForImgui();
		SIZE GetWinSize();

		void SetScene();

	};
}