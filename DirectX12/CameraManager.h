#pragma once
#include "Vector3.h"
#include <memory>

namespace KochaEngine
{
	class Camera;
	class Player;

	//�J�����̏�ԊǗ�
	enum CameraState
	{
		DEFAULT_CAMERA_STATE,
		BATTLE_CAMERA_STATE,
	};

	class CameraManager
	{
	private:
		std::weak_ptr<Camera> camera;
		CameraState cameraState;

		Player* player;

		//���ꂼ��̃J�����ʒu
		Vector3 defaultEyePosition;
		Vector3 currentEyePosition;
		Vector3 battleEyePosition;
		//���ꂼ��̃^�[�Q�b�g�ʒu
		Vector3 defaultTargetPosition;
		Vector3 currentTargetPosition;
		Vector3 battleTargetPosition;

		//�f�t�H���g��Ԃ̍X�V����
		void DefaultCameraUpdate();
		//�o�g����Ԃ̍X�V����
		void BattleCameraUpdate();

	public:
		CameraManager(std::weak_ptr<Camera> arg_camera);
		~CameraManager();

		void Initialize();
		void Update();

		void SetDefaultPosition(); //�o�g�����ɃJ�������f�t�H���g�̈ʒu�ɖ߂�
		inline void SetPlayer(Player* arg_player) { player = arg_player; }
		inline void SetCameraState(const CameraState& arg_cameraState) { cameraState = arg_cameraState; }
		inline void SetBattleEyePosition(const Vector3& arg_position) { battleEyePosition = arg_position; }
		inline void SetBattleEyePositionX(const float arg_positionX) { battleEyePosition.x = arg_positionX; }
		inline void SetBattleEyePositionZ(const float arg_positionZ) { battleEyePosition.z = arg_positionZ; }
		inline void MoveBattleEyePositionX(const float arg_moveX) { battleEyePosition.x += arg_moveX; }
		inline void MoveBattleEyePositionZ(const float arg_moveZ) { battleEyePosition.z += arg_moveZ; }
		inline void SetBattleTargetPosition(const Vector3& arg_position) { battleTargetPosition = arg_position; }
		inline void SetBattleTargetPositionX(const float arg_positionX) { battleTargetPosition.x = arg_positionX; }
		inline void SetBattleTargetPositionZ(const float arg_positionZ) { battleTargetPosition.z = arg_positionZ; }
		inline void MoveBattleTargetPositionX(const float arg_moveX) { battleTargetPosition.x += arg_moveX; }
		inline void MoveBattleTargetPositionZ(const float arg_moveZ) { battleTargetPosition.z += arg_moveZ; }
		inline void SetCameraShake(const float arg_power) { currentEyePosition.z += arg_power; }
	};
}
