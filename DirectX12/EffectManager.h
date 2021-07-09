#pragma once
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>

namespace KochaEngine
{
	class Dx12_Wrapper;
	class EffectManager
	{
	private:
		//エフェクトレンダラー
		EffekseerRenderer::RendererRef _efkRenderer;
		//エフェクトマネージャー
		Effekseer::ManagerRef _efkManager;

		//メモリプール
		Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> _efkMemoryPool;
		//コマンドリスト
		Effekseer::RefPtr<EffekseerRenderer::CommandList> _efkCmdList;

		//エフェクト再生に必要なもの
		//エフェクト本体
		Effekseer::EffectRef _effect;
		//エフェクトハンドル
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