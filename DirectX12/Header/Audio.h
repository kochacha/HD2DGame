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
		// �{�C�X�����p�X�̊J�n��
		//STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
		void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
		// �{�C�X�����p�X�̏I���� /
		STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
		// �o�b�t�@�X�g���[���̍Đ����I��������
		STDMETHOD_(void, OnStreamEnd) (THIS) {};
		// �o�b�t�@�̎g�p�J�n��
		STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
		// �o�b�t�@�̖����ɒB������
		STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
			// �o�b�t�@���������
			delete[] pBufferContext;
		};
		// �Đ������[�v�ʒu�ɒB������
		STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
		// �{�C�X�̎��s�G���[��
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

		//�`�����N�w�b�_
		struct Chunk
		{
			char id[4]; //�`�����N����ID
			int32_t size; //�`�����N�T�C�Y
		};
		//RIFF�w�b�_�`�����N
		struct RiffHeader
		{
			Chunk chunk; //"RIFF"
			char type[4]; //"WAVE"
		};
		//FMT�`�����N
		struct FormatChunk
		{
			Chunk chunk; //"fmt"
			WAVEFORMAT fmt; //�g�`�t�H�[�}�b�g
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
