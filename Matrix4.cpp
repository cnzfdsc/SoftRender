#include "InternalHeader.h"
#include "SRHeader.h"

const Matrix4 Matrix4::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );

Matrix4::Matrix4( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 )
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

Matrix4& Matrix4::Identity()
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

	return *this;
}

Matrix4 Matrix4::CalcRotationMatrix( const Vector3& axis, float r, const Vector3& translation )
{
	float sinvalue = ::sinf( r ), cosvalue = ::cosf( r ), cosreverse = 1.0f - cosvalue;

	Vector3 n( axis );
	n.Normalize( );

	Matrix4 rMtx;

	rMtx.m[0][0] = cosreverse * n.x * n.x + cosvalue;
	rMtx.m[0][1] = cosreverse * n.x * n.y + sinvalue * n.z;
	rMtx.m[0][2] = cosreverse * n.x * n.z - sinvalue * n.y;
	rMtx.m[0][3] = 0.0f;

	rMtx.m[1][0] = cosreverse * n.x * n.y - sinvalue * n.z;
	rMtx.m[1][1] = cosreverse * n.y * n.y + cosvalue;
	rMtx.m[1][2] = cosreverse * n.y * n.z + sinvalue * n.x;
	rMtx.m[1][3] = 0.0f;

	rMtx.m[2][0] = cosreverse * n.x * n.z + sinvalue * n.y;
	rMtx.m[2][1] = cosreverse * n.y * n.z - sinvalue * n.x;
	rMtx.m[2][2] = cosreverse * n.z * n.z + cosvalue;
	rMtx.m[2][3] = 0.0f;

	rMtx.m[3][0] = translation.x;
	rMtx.m[3][1] = translation.y;
	rMtx.m[3][2] = translation.z;
	rMtx.m[3][3] = 1.0f;

	return rMtx;
}

Matrix4 Matrix4::CalcViewMatrix( const Vector3& eye, const Vector3& lookat, const Vector3& up )
{
	Vector3 zaxis = ( lookat - eye ).Normalize( );
	Vector3 xaxis = Vector3::Cross( up, zaxis ).Normalize( );
	Vector3 yaxis = Vector3::Cross( zaxis, xaxis );

	float xeye = - Vector3::Dot( xaxis, eye );
	float yeye = - Vector3::Dot( yaxis, eye );
	float zeye = - Vector3::Dot( zaxis, eye );

	return Matrix4(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		xeye,    yeye,    zeye, 1.0f );
}

Matrix4 Matrix4::CalcProjMatrix( float fovy, float ratio, float nearz, float farz )
{
	float cotfov = 1 / ::tanf( fovy / 2 );
	float zratio = farz / ( farz - nearz );
	return Matrix4(
		cotfov / ratio,	 0,		 0,				  0,
		0,				 cotfov, 0,				  0,
		0,				 0,		 zratio,		  1.0f,
		0,				 0,		 -nearz * zratio, 0.0f );
}

Matrix4& Matrix4::Translation( float x, float y, float z )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] =    x; m[3][1] =    y; m[3][2] =    z; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Translation( const Vector3& t )
{
	return Translation( t.x, t.y, t.z );
}

Matrix4& Matrix4::Rotation( const Vector3& axis, float r )
{
	float sinvalue = ::sinf( r ), cosvalue = ::cosf( r ), cosreverse = 1.0f - cosvalue;

	Vector3 n( axis );
	n.Normalize( );

	m[0][0] = cosreverse * n.x * n.x + cosvalue;
	m[0][1] = cosreverse * n.x * n.y + sinvalue * n.z;
	m[0][2] = cosreverse * n.x * n.z - sinvalue * n.y;
	m[0][3] = 0.0f;

	m[1][0] = cosreverse * n.x * n.y - sinvalue * n.z;
	m[1][1] = cosreverse * n.y * n.y + cosvalue;
	m[1][2] = cosreverse * n.y * n.z + sinvalue * n.x;
	m[1][3] = 0.0f;

	m[2][0] = cosreverse * n.x * n.z + sinvalue * n.y;
	m[2][1] = cosreverse * n.y * n.z - sinvalue * n.x;
	m[2][2] = cosreverse * n.z * n.z + cosvalue;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::operator *= ( const Matrix4& mat )
{
	float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3], m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3],
		m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3], m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

	m[0][0] = m00 * mat.m[0][0] + m01 * mat.m[1][0] + m02 * mat.m[2][0] + m03 * mat.m[3][0];
	m[0][1] = m00 * mat.m[0][1] + m01 * mat.m[1][1] + m02 * mat.m[2][1] + m03 * mat.m[3][1];
	m[0][2] = m00 * mat.m[0][2] + m01 * mat.m[1][2] + m02 * mat.m[2][2] + m03 * mat.m[3][2];
	m[0][3] = m00 * mat.m[0][3] + m01 * mat.m[1][3] + m02 * mat.m[2][3] + m03 * mat.m[3][3];

	m[1][0] = m10 * mat.m[0][0] + m11 * mat.m[1][0] + m12 * mat.m[2][0] + m13 * mat.m[3][0];
	m[1][1] = m10 * mat.m[0][1] + m11 * mat.m[1][1] + m12 * mat.m[2][1] + m13 * mat.m[3][1];
	m[1][2] = m10 * mat.m[0][2] + m11 * mat.m[1][2] + m12 * mat.m[2][2] + m13 * mat.m[3][2];
	m[1][3] = m10 * mat.m[0][3] + m11 * mat.m[1][3] + m12 * mat.m[2][3] + m13 * mat.m[3][3];

	m[2][0] = m20 * mat.m[0][0] + m21 * mat.m[1][0] + m22 * mat.m[2][0] + m23 * mat.m[3][0];
	m[2][1] = m20 * mat.m[0][1] + m21 * mat.m[1][1] + m22 * mat.m[2][1] + m23 * mat.m[3][1];
	m[2][2] = m20 * mat.m[0][2] + m21 * mat.m[1][2] + m22 * mat.m[2][2] + m23 * mat.m[3][2];
	m[2][3] = m20 * mat.m[0][3] + m21 * mat.m[1][3] + m22 * mat.m[2][3] + m23 * mat.m[3][3];

	m[3][0] = m30 * mat.m[0][0] + m31 * mat.m[1][0] + m32 * mat.m[2][0] + m33 * mat.m[3][0];
	m[3][1] = m30 * mat.m[0][1] + m31 * mat.m[1][1] + m32 * mat.m[2][1] + m33 * mat.m[3][1];
	m[3][2] = m30 * mat.m[0][2] + m31 * mat.m[1][2] + m32 * mat.m[2][2] + m33 * mat.m[3][2];
	m[3][3] = m30 * mat.m[0][3] + m31 * mat.m[1][3] + m32 * mat.m[2][3] + m33 * mat.m[3][3];

	return *this;
}

Matrix4 Matrix4::operator * ( const Matrix4& mat ) const
{
	Matrix4 rm( *this );
	rm *= mat;

	return rm;
}

void Matrix4::Inverse( )
{
	int i, j, k;
	double d, p;
	int is[4];
	int js[4];
	for ( k = 0; k < 4; k ++ )
	{
		d = 0.0;
		for ( i = k; i < 4; i ++ )
		{
			for ( j = k; j < 4; j ++ )
			{
				p = ::fabs( m[i][j] );
				if ( p > d )
				{
					d = p;
					is[k] = i;
					js[k] = j;
				}
			}

			if(d + 1.0 == 1.0)
				return;

			if ( is[k] != k )
				for ( j = 0; j < 4; j ++ )
				{
					p = m[k][j];
					m[k][j] = m[is[k]][j];
					m[is[k]][j] = (float) p;
				}

			if ( js[k] != k )
			{
				for ( i = 0; i < 4; i ++ )
				{
					p = m[i][k];
					m[i][k] = m[i][js[k]];
					m[i][js[k]] = (float) p;
				}
			}

			m[k][k] = 1.0f / m[k][k];
			for ( j = 0; j < 4; j ++ )
				if ( j != k )
					m[k][j] = m[k][j] * m[k][k];

			for ( i = 0; i < 4; i ++ )
				if ( i != k )
					for ( j = 0; j < 4; j ++ )
						if ( j != k )
							m[i][j] = m[i][j] - m[i][k] * m[k][j];

			for ( i = 0; i < 4; i ++ )
				if ( i != k )
					m[i][k]=-m[i][k]*m[k][k];
		}
	}

	for ( k = 4 - 1; k >= 0; k -- )
	{
		if ( js[k] != k )
		{
			for ( j = 0; j < 4; j ++ )
			{
				p = m[k][j];
				m[k][j] = m[js[k]][j];
				m[js[k]][j] = (float) p;
			}
		}

		if ( is[k] != k )
		{
			for ( i = 0; i < 4; i ++ )
			{
				p = m[i][k];
				m[i][k] = m[i][is[k]];
				m[i][is[k]] = (float) p;
			}
		}
	}
}