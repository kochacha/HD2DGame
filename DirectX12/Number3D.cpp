#include "Number3D.h"
#include "Object.h"
#include "Util.h"

KochaEngine::Number3D::Number3D(const Vector3& arg_position, const Vector3& arg_scale)
{
	position = arg_position;
	scale = arg_scale;

	for (int i = 0; i < NUM3D_MAX_DIGIT; i++)
	{
		planes[i] = new Object("plane");
	}

	Initialize();
}

KochaEngine::Number3D::Number3D(const int arg_num, const Vector3& arg_position, const Vector3& arg_scale)
{
	position = arg_position;
	scale = arg_scale;

	for (int i = 0; i < NUM3D_MAX_DIGIT; i++)
	{
		planes[i] = new Object("plane");
	}

	Initialize();

	//�������Z�b�e�B���O
	SetNumber(arg_num);
}

KochaEngine::Number3D::~Number3D()
{
	Remove();
}

void KochaEngine::Number3D::Initialize()
{
	for (int i = 0; i < NUM3D_MAX_DIGIT; i++)
	{
		planes[i]->SetScale(Vector3(-scale.x, scale.y, scale.z));
		planes[i]->SetBillboardType(Object::BILLBOARD_Y);
	}
	preNumber = -1; //���̐����ȊO
}

void KochaEngine::Number3D::Draw(Camera* arg_camera, LightManager* arg_ligtManager)
{
	//�ʒu�̏C��
	FixPosition();

	//�`��
	auto remainder = NUM3D_MAX_DIGIT - digit;
	for (int i = 0; i < digit; i++)
	{
		planes[remainder + i]->Draw(arg_camera, arg_ligtManager);
	}
}

void KochaEngine::Number3D::Draw(const int arg_number, Camera* arg_camera, LightManager* arg_ligtManager)
{
	//�������Z�b�e�B���O
	SetNumber(arg_number);
	//�ʒu�̏C��
	FixPosition();

	//�`��
	auto remainder = NUM3D_MAX_DIGIT - digit;
	for (int i = 0; i < digit; i++)
	{
		planes[remainder + i]->Draw(arg_camera, arg_ligtManager);
	}
}

void KochaEngine::Number3D::Remove()
{
	for (int i = 0; i < NUM3D_MAX_DIGIT; i++)
	{
		delete planes[i];
	}
}

void KochaEngine::Number3D::SetNumber(const int arg_number)
{
	int number = arg_number;
	if (number > 9999999) number = 9999999;
	if (number < 0) number = 0;

	//���������Ȃ�X�V���Ȃ�
	if (arg_number == preNumber) return;
	
	preNumber = number;
	digit = Util::GetDigit(number);

	int digits = digit - 1;
	int hoge = (int)std::pow(10, digits);
	int baka = number;

	for (int i = 0; i <= digits; i++)
	{
		std::string extension = std::to_string(baka / hoge) + ".png";
		planes[NUM3D_MAX_DIGIT - digits + i - 1]->SetTexture("Resources/Texture/Font/number_" + extension);
		baka = baka % hoge;
		hoge /= 10;
	}

}

void KochaEngine::Number3D::FixPosition()
{
	//�������������ǂ���
	bool isEven = false;
	if (digit % 2 == 0)
	{
		isEven = true;
	}

	float fixX = 0;
	if (isEven)
	{
		fixX = scale.x * (digit / 2);
	}
	else
	{
		fixX = scale.x * ((digit - 1) / 2) + scale.x / 2.0f;
	}

	auto remainder = NUM3D_MAX_DIGIT - digit;
	for (int i = 0; i < digit; i++)
	{
		Vector3 _position = Vector3(position.x + scale.x * i - fixX, position.y, position.z);
		planes[remainder + i]->SetPosition(_position);
	}
}
