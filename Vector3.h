#pragma once

struct Vector4;
struct Matrix4;

struct Vector3
{
	static float Distance( const Vector3& v1, const Vector3& v2 );

	float x, y, z;

	inline Vector3( ) : x( 0 ), y( 0 ), z( 0 )
	{}

	inline Vector3( float xx, float yy, float zz ) : x( xx ), y( yy ), z( zz )
	{}

	Vector3( const Vector4& v4 );

	inline static Vector3 Cross( const Vector3& v1, const Vector3& v2 )
	{
		return Vector3( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
	}

	inline static float Dot( const Vector3& v1, const Vector3& v2 )
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline Vector3& operator *= ( float s )
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	inline Vector3 operator * ( float s )
	{
		Vector3 v( *this );
		return v *= s;
	}

	inline Vector3 operator * ( const Matrix4& m ) const
	{
		Vector3 r( *this );
		return r *= m;
	}

	inline Vector3 operator - ( )
	{
		return Vector3( -x, -y, -z );
	}

	inline Vector3& operator -= ( const Vector3& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3 operator - ( const Vector3& v ) const;

	inline Vector3& operator += ( const Vector3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vector3 operator + ( const Vector3& v ) const
	{
		return Vector3( x + v.x, y + v.y, z + v.z );
	}
	
	float Magnitude( );
	Vector3& Normalize( );
	Vector3& operator *= ( const Matrix4& m );
};