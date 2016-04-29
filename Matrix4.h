#pragma once

struct Vector3;

struct Matrix4
{
	const static Matrix4 cIdentity;

	float m[4][4];

	inline Matrix4( )
	{ }

	Matrix4( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 );

	Matrix4& Identity();

	static Matrix4 CalcRotationMatrix( const Vector3& axis, float r, const Vector3& translation );
	static Matrix4 CalcViewMatrix( const Vector3& eye, const Vector3& lookat, const Vector3& up );
	static Matrix4 CalcProjMatrix( float fovy, float ratio, float nearz, float farz );

	Matrix4& Translation( float x, float y, float z );
	Matrix4& Translation( const Vector3& t );
	Matrix4& Rotation( const Vector3& axis, float r );
	Matrix4& operator *= ( const Matrix4& mat );
	Matrix4 operator * ( const Matrix4& mat ) const;

	void Matrix4::Inverse( );
};