#pragma once
#include <DirectXMath.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<vector>
#include<d3dcompiler.h>
#include<string>
#include "PostEffect.h"
#include "Vector3.h"
#include "Vector4.h"

#define OBJ_COUNT 25
#define FBX_COUNT 1

namespace KochaEngine
{
	class Window;
	class Dx12_Wrapper;
	class Dx12_Blob;
	class Dx12_Descriptor;
	class Dx12_RootSignature;
	class Dx12_Pipeline;
	class SceneManager;
	class Texture2D;
	class Object;
	class PMDModel;
	class Camera;
	class FBXObject;
	class FBXModel;
	class EffectManager;
	class LightManager;

	class Application
	{
	private:
		HWND hwnd;
		Application();
		Application(const Application&) = delete;
		void operator=(const Application&) = delete;
		Window* window;
		Dx12_Wrapper* dx12;
		Dx12_Descriptor* descriptor;
		Dx12_Blob* blob;
		Dx12_RootSignature* rootSignature;
		Dx12_Pipeline* pipeline;
		SceneManager* sceneManager;
		Camera* camera;
		Camera* lightCamera;

		Texture2D* texture[1];
		Object* obj[OBJ_COUNT];
		Object* floor;
		Object* taimatu;
		Object* skyObj;

		FBXModel* fbxModel = nullptr;
		FBXObject* fbxObject[FBX_COUNT];

		PostEffect* peraBloom;
		PostEffect* peraEffect;
		PostEffect* peraDof;

		EffectManager* effectManager;
		LightManager* lightManager;

		ShaderType peraEffectType;
		bool isDof;

		float vignetteScale;
		float gBoyPixelSize;
		float mosaicSize;
		float sepiaScale;
		float blurScale;
		float cAbeScale;

		float color[4];
		Vector4 shaderColor;

		bool isActiveDirLight;
		Vector3 dirLightDirection;
		Vector3 dirLightColor;
		Vector3 pointLightPosition;
		Vector3 pointLightColor;
		Vector3 pointLightAtten;

		void Load();
		void LoadScene();
		void InitFPS();
		void DrawGUI();
		void MeraMera();
		bool UpdateFPS();

		const double MIN_FRAME_TIME = 1.000000f / 60.000000f;
		double frameTime = 0.000000f;
		double fps = 0.000000f;
		LARGE_INTEGER timeStart;
		LARGE_INTEGER timeEnd;
		LARGE_INTEGER timeFreq;

	public:
		~Application();
		static Application& Instance();
		SIZE GetWindowSize()const;
		bool Initialize();
		void Run();
		void Terminate();

		static float clearColor[4];
	};
}