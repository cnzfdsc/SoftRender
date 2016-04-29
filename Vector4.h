#pragma once

struct Vector3;
struct Matrix4;

struct Vector4
{
	const static Vector4 cZero;

	static float Distance( const Vector4& v1, const Vector4& v2 );

	float x, y, z, w;

	inline Vector4( ) : x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{}

	inline Vector4( float xx, float yy, float zz ) : x( xx ), y( yy ), z( zz ), w( 1 )
	{}

	inline Vector4( float xx, float yy, float zz, float ww ) : x( xx ), y( yy ), z( zz ), w( ww )
	{}

	Vector4( const Vector3& v3 );
	Vector4( const Vector3& v3, float w );

	inline static Vector4 Cross( const Vector4& v1, const Vector4& v2 )
	{
		Vector4 tv2( v2 );
		tv2.w = v2.w == 0.f ? 1.f : v2.w;
		if ( v1.w != v2.w )
		{
			float t = v1.w / v2.w;
			tv2 *= t;
		}
		return Vector4( v1.y * tv2.z - v1.z * tv2.y, v1.z * tv2.x - v1.x * tv2.z, v1.x * tv2.y - v1.y * tv2.x, 0 );
	}

	inline static float Dot( const Vector4& v1, const Vector4& v2 )
	{
		Vector4 tv2( v2 );
		tv2.w = v2.w == 0.f ? 1.f : v2.w;
		if ( v1.w != v2.w )
		{
			float t = v1.w / v2.w;
			tv2 *= t;
		}
		return v1.x * tv2.x + v1.y * tv2.y + v1.z * tv2.z;
	}

	inline Vector4& operator *= ( float s )
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	inline Vector4 operator * ( float s )
	{
		Vector4 v( *this );
		return v *= s;
	}

	Vector4& operator *= ( const Matrix4& m );
	inline Vector4 operator * ( const Matrix4& m ) const
	{
		Vector4 r( *this );
		return r *= m;
	}

	inline Vector4 operator - ( )
	{
		return Vector4( -x, -y, -z );
	}

	inline Vector4& operator -= ( const Vector4& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	inline Vector4 operator - ( const Vector4& v ) const
	{
		Vector4 r( *this );
		return r -= v;
	}

	inline Vector4& operator += ( const Vector4& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	inline Vector4 operator + ( const Vector4& v ) const
	{
		return Vector4( x + v.x, y + v.y, z + v.z, w + v.w );
	}

	inline bool operator == ( const Vector4& v ) const
	{
		Vector4 vv( v );
		vv.w = v.w == 0.f ? 1.f : v.w;
		if ( this->w != v.w )
		{
			float t = this->w / v.w;
			vv *= t;
		}

		return x == vv.x && y == vv.y && z == vv.z;
	}
	
	float Magnitude( );
	Vector4& Normalize( );
	void PerspectiveDivision( );
};