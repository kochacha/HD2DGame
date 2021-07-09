#pragma once
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>

namespace KochaEngine
{
	class Dx12_Wrapper;
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
		Effekseer::EffectRef _effect;
		//�G�t�F�N�g�n���h��
		Effekseer::Handle _efkHandle;

		Dx12_Wrapper& dx12;

		void Initialize();

	public:
		EffectManager(Dx12_Wrapper& dx12);
		~EffectManager();

		void LoadEffect();
		void Play();
		void Update();
	};
}