#pragma once
#include <xaudio2.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#pragma comment(lib,"xaudio2.lib")

namespace KochaEngine
{
	struct SoundData
	{
		WAVEFORMATEXTENSIBLE wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };
	};

	class XAudio2VoiceCallback : public IXAudio2VoiceCallback
	{
	public:
		// ボイス処理パスの開始時
		//STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
		void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
		// ボイス処理パスの終了時 /
		STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
		// バッファストリームの再生が終了した時
		STDMETHOD_(void, OnStreamEnd) (THIS) {};
		// バッファの使用開始時
		STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
		// バッファの末尾に達した時
		STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
			// バッファを解放する
			delete[] pBufferContext;
		};
		// 再生がループ位置に達した時
		STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
		// ボイスの実行エラー時
		STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
	};

	class Audio
	{
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		XAudio2VoiceCallback voiceCallback;
		ComPtr<IXAudio2> xAudio2;
		IXAudio2MasteringVoice* masterVoice;
		IXAudio2SourceVoice* pSourceVoice;

		//////////////
		WAVEFORMATEXTENSIBLE wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };

		//チャンクヘッダ
		struct Chunk
		{
			char id[4]; //チャンク毎のID
			int32_t size; //チャンクサイズ
		};
		//RIFFヘッダチャンク
		struct RiffHeader
		{
			Chunk chunk; //"RIFF"
			char type[4]; //"WAVE"
		};
		//FMTチャンク
		struct FormatChunk
		{
			Chunk chunk; //"fmt"
			WAVEFORMAT fmt; //波形フォーマット
		};



		HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
		HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
	public:
		Audio();
		~Audio();

		void Init();
		void LoadSound();


		void PlayWave(const char* strFileName, float volume);
		void LoopPlayWave(const char* strFileName, float volume);
		void Stop();
		void Pause();
		void ReStart();
		void SetVolume(float volume);

		void SetPlaySpeed(float speed);
	};
}
