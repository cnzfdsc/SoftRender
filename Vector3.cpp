#include "InternalHeader.h"
#include "SRHeader.h"

Vector3::Vector3( const Vector4& v4 ) : x( v4.x ), y( v4.y ), z( v4.z )
{
}

float Vector3::Distance( const Vector3& v1, const Vector3& v2 )
{
	return ::sqrt( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) + ( v1.z - v2.z ) * ( v1.z - v2.z ) );
}

Vector3 Vector3::operator - ( const Vector3& v ) const
{
	Vector4 r( *this );
	return r -= v;
}

float Vector3::Magnitude( )
{
	return ::sqrt( x * x + y * y + z * z );
}

Vector3& Vector3::Normalize( )
{
	float m = Magnitude( );
	x = x / m;
	y = y / m;
	z = z / m;

	return *this;
}

Vector3& Vector3::operator *= ( const Matrix4& m )
{
	float xx = x, yy = y, zz = z;
	float homo = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + m.m[3][3];
	//// 几乎不可能, 我只是突然想到这里了, 加上吧
	if ( homo == 0.0f )
		homo = 0.000001f;
	x = ( xx * m.m[0][0] + yy * m.m[1][0] + zz * m.m[2][0] + 1.0f * m.m[3][0] ) / homo;
	y = ( xx * m.m[0][1] + yy * m.m[1][1] + zz * m.m[2][1] + 1.0f * m.m[3][1] ) / homo;
	z = ( xx * m.m[0][2] + yy * m.m[1][2] + zz * m.m[2][2] + 1.0f * m.m[3][2] ) / homo;

	return *this;
}