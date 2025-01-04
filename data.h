#pragma once

struct Vector
{
	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;
};

struct Matrix
{
	Vector m_i = { 1.0f, 0.0f, 0.0f };
	Vector m_j = { 0.0f, 1.0f, 0.0f };
	Vector m_k = { 0.0f, 0.0f, 1.0f };
};

struct Quaternion
{
	float m_w = 1.0f;
	Vector m_real;
};

struct Transform
{
	Quaternion m_rotation;
	Vector m_position;
};

struct Kinematic
{
	Vector m_source;
	Vector m_target;
	float m_maxSpeed = 0.0f;
	float m_maxAcceleration = 0.0f;
	float m_maxRotation = 0.0f;
	float m_time = 0.0f;
};

struct RigidBody
{
	Vector m_position;
	Vector m_linearVelocity;
	Vector m_angularVelocity;
	Matrix m_intertiaTensor;
	Matrix m_invIntertiaTensor;
	float m_mass = 0.0f;
	float m_principalMomentsIntertia = 0.0f;
};

struct Health
{
	// this because I want to groupBy this
	using FieldType = float;

	float m_maxValue = 0.0f;
	float m_currentValue = 0.0f;
};

struct Camera
{
};

struct Render
{
};