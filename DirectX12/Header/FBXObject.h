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

		//�萔�o�b�t�@�p�f�[�^�\����
		struct ConstBufferDataTransform
		{
			DirectX::XMMATRIX viewProj;
			DirectX::XMMATRIX world;
			Vector3 cameraPos;
		};

		//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
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

		//���f���̃Z�b�g
		void SetModel(FBXModel* model) { this->model = model; }

		static void SetDevice(ID3D12Device* device) { FBXObject::device = device; }
		static void SetCamera(Camera* camera) { FBXObject::camera = camera; }

	protected:

		ComPtr<ID3D12Resource> constBuffTransform;
		ComPtr<ID3D12Resource> constBuffSkin;

		//���[�J���X�P�[��
		Vector3 scale = { 1,1,1 };
		//X,Y,Z������̃��[�J����]�p
		Vector3 rotation = { 0,0,0 };
		//���[�J�����W
		Vector3 position = { 0,0,0 };
		//���[�J�����[���h�ϊ��s��
		DirectX::XMMATRIX matWorld;
		//���f��
		FBXModel* model = nullptr;

	private:
		//1�t���[���̎���
		FbxTime frameTime;
		//�A�j���[�V�����J�n����
		FbxTime startTime;
		//�A�j���[�V�����I������
		FbxTime endTime;
		//���ݎ���(�A�j���[�V����)
		FbxTime currentTime;
		//�A�j���[�V�����Đ���
		bool isPlay = false;

		void Initialize();

		//�f�o�C�X
		static ID3D12Device* device;
		//�J����
		static Camera* camera;
	};
}