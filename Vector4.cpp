#include "InternalHeader.h"
#include "SRHeader.h"

const Vector4 Vector4::cZero(0.0f, 0.0f, 0.0f, 0.0f);

float Vector4::Distance( const Vector4& v1, const Vector4& v2 )
{
	return ::sqrt( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) + ( v1.z - v2.z ) * ( v1.z - v2.z ) );
}

Vector4::Vector4( const Vector3& v3 ) : x( v3.x ), y( v3.y ), z( v3.z ), w( 0 )
{
}

Vector4::Vector4( const Vector3& v3, float w ) : x( v3.x ), y( v3.y ), z( v3.z ), w( w )
{
}

float Vector4::Magnitude( )
{
	if ( w == 1 )
		assert( ( "Magnitude on a vertex", 0 ) );
	return ::sqrt( x * x + y * y + z * z );
}

Vector4& Vector4::Normalize( )
{
	float m = Magnitude( );
	x = x / m;
	y = y / m;
	z = z / m;

	return *this;
}

void Vector4::PerspectiveDivision( )
{
	x /= w;
	y /= w;
	z /= w;
}

Vector4& Vector4::operator *= ( const Matrix4& m )
{
	float xx = x, yy = y, zz = z;
	x = ( xx * m.m[0][0] + yy * m.m[1][0] + zz * m.m[2][0] + 1.0f * m.m[3][0] );
	y = ( xx * m.m[0][1] + yy * m.m[1][1] + zz * m.m[2][1] + 1.0f * m.m[3][1] );
	z = ( xx * m.m[0][2] + yy * m.m[1][2] + zz * m.m[2][2] + 1.0f * m.m[3][2] );
	w = ( xx * m.m[0][3] + yy * m.m[1][3] + zz * m.m[2][3] + 1.0f * m.m[3][3] );

	return *this;
}