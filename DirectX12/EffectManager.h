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
		std::map<std::string, Effekseer::EffectRef> _effect;
		//エフェクトハンドル
		Effekseer::Handle _efkHandle;

		Dx12_Wrapper& dx12;

		void Initialize();

	public:
		EffectManager(Dx12_Wrapper& dx12);
		~EffectManager();

		void LoadEffect(const std::string& arg_efkPath, const float arg_scale);
		void Play(const std::string& arg_efkName, const Vector3& arg_position);
		void Update(Camera* camera);

		void SetPosition(const Vector3& arg_position);
		void SetScale(const float arg_scale);
	};
}