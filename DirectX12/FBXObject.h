#pragma once
#include "FBXModel.h"
#include "Camera.h"
#include "Vector3.h"
#include "Vector4.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

namespace KochaEngine
{
	class FBXObject
	{
	protected:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		static const int MAX_BONES = 32;

		//定数バッファ用データ構造体
		struct ConstBufferDataTransform
		{
			DirectX::XMMATRIX viewProj;
			DirectX::XMMATRIX world;
			Vector3 cameraPos;
		};

		//定数バッファ用データ構造体(スキニング)
		struct ConstBufferDataSkin
		{
			DirectX::XMMATRIX bones[MAX_BONES];
		};

		FBXObject();
		~FBXObject();

		void Draw(ID3D12GraphicsCommandList* cmdList);

		void PlayAnimation();

		void SetPosition(const Vector3 arg_position);

		void MovePosition(const Vector3 move);
		void MoveScale(const Vector3 moveScale);
		void MoveRotate(const Vector3 moveRotate);

		//モデルのセット
		void SetModel(FBXModel* model) { this->model = model; }

		static void SetDevice(ID3D12Device* device) { FBXObject::device = device; }
		static void SetCamera(Camera* camera) { FBXObject::camera = camera; }

	protected:

		ComPtr<ID3D12Resource> constBuffTransform;
		ComPtr<ID3D12Resource> constBuffSkin;

		//ローカルスケール
		Vector3 scale = { 1,1,1 };
		//X,Y,Z軸周りのローカル回転角
		Vector3 rotation = { 0,0,0 };
		//ローカル座標
		Vector3 position = { 0,0,0 };
		//ローカルワールド変換行列
		DirectX::XMMATRIX matWorld;
		//モデル
		FBXModel* model = nullptr;

	private:
		//1フレームの時間
		FbxTime frameTime;
		//アニメーション開始時間
		FbxTime startTime;
		//アニメーション終了時間
		FbxTime endTime;
		//現在時間(アニメーション)
		FbxTime currentTime;
		//アニメーション再生中
		bool isPlay = false;

		void Initialize();

		//デバイス
		static ID3D12Device* device;
		//カメラ
		static Camera* camera;
	};
}