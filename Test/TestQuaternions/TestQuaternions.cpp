// TestQuaternions.cpp : Defines the entry point for the console application.
//

#include <Math/Quaternion.h>
#include <Windows.h>
#include <sstream>
#include <iostream>


bool eq( float a, float b )
{
	return fabsf( a - b ) < 0.01;
}
void TestYpr( float y, float p, float r)
{
	std::stringstream ss;
	Vect3f ypr( y*M_PI / 180, p*M_PI / 180, r*M_PI / 180 );
	Quatf q = Quatf::GetQuaternionFromRadians( ypr );
	Vect3f ypr2 = Quatf::GetEulerFromQuaternion( q );
	Quatf q2 = Quatf::GetQuaternionFromRadians( ypr2 );
	if ( eq( q.x, q2.x ) && eq( q.y, q2.y ) && eq( q.z, q2.z ) && eq( q.w, q2.w ) )
	{
		ss << "ok\n";
	}
	else
	{
		ss << "Error in (" << y << ", " << p << ", " << r << ") -> ("
			<< ypr2.x*180/M_PI << ", " << ypr2.y*180/M_PI << ", " << ypr2.z*180/M_PI << ")\n\t\t<"
			<< q.x << ", " << q.y << ", " << q.z << ", " << q.w << "> => <"
			<< q2.x << ", " << q2.y << ", " << q2.z << ", " << q2.w << ">\n";
	}
	OutputDebugStringA( ss.str().c_str() );
}



int main()
{
	TestYpr( 0, 0, 0 );
	TestYpr( 90, 0, 0 );
	TestYpr( 0, 90, 0 );
	TestYpr( 0, 0, 90 );
	TestYpr( 90, 90, 0 );
	TestYpr( 0, 90, 90 );
	TestYpr( 90, 0, 90 );
	TestYpr( 90, 45, 0 );
	TestYpr( 0, 45, 90 );
	TestYpr( 45, 90, 0 );
	TestYpr( 45, 0, 90 );
	TestYpr( 90, 0, 45 );
	TestYpr( 0, 90, 45 );
	TestYpr( 20, 10, 30 );
	TestYpr( 120, 10, 30 );
	TestYpr( 20, 10, -130 );
	TestYpr( 20, -50, 30 );
	TestYpr( 20, 90, -130 );
	TestYpr( -110, 90, 30 );

	getchar();
    return 0;
}

