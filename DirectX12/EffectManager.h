#pragma once
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#include <map>
#include "Vector3.h"

namespace KochaEngine
{
	class Dx12_Wrapper;
	class Camera;
	class EffectManager
	{
	private:
		//�G�t�F�N�g�����_���[
		EffekseerRenderer::RendererRef _efkRenderer;
		//�G�t�F�N�g�}�l�[�W���[
		Effekseer::ManagerRef _efkManager;

		//�������v�[��
		Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> _efkMemoryPool;
		//�R�}���h���X�g
		Effekseer::RefPtr<EffekseerRenderer::CommandList> _efkCmdList;

		//�G�t�F�N�g�Đ��ɕK�v�Ȃ���
		//�G�t�F�N�g�{��
		std::map<std::string, Effekseer::EffectRef> _effect;
		//�G�t�F�N�g�n���h��
		Effekseer::Handle _efkHandle;

		Dx12_Wrapper& dx12;

		void Initialize();

	public:
		EffectManager(Dx12_Wrapper& dx12);
		~EffectManager();

		void LoadEffect(const std::string& arg_efkPath, const float arg_scale);
		void Play(const std::string& arg_efkName, const Vector3& arg_position);
		void Update(Camera* camera);
	};
}