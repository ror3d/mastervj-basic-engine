#pragma once

#ifndef __QUATERNION__
#define __QUATERNION__

#include "MathUtils.h"
#include "MathTypes.h"
#include "Vector3.h"
#include "Vector4.h"


template<typename T>
class Quaternion
{
	// member variables
public:
	T x, y, z, w;

	// constructors/destructor
public:
	inline Quaternion() : x(Zero<T>()), y(Zero<T>()), z(Zero<T>()), w(One<T>()){};
	inline Quaternion(const Quaternion& q): x(q.x), y(q.y), z(q.z), w(q.w) {};
	inline Quaternion(T qx, T qy, T qz, T qw): x(qx), y(qy), z(qz), w(qw) {};
	inline Quaternion(Vector3<T> axis, T angle)
         {
           T sinus   = (T) sin(angle * Half<T>());
           T cosinus = (T) cos(angle * Half<T>());
           x = axis.x * sinus;
           y = axis.y * sinus;
           z = axis.z * sinus;
           w = cosinus;
         };
	inline Quaternion(Matrix33<T> mat)
         {
           T trace = mat.m00 + mat.m11 + mat.m22;
           //check the diagonal
           if(trace > Zero<T>())
           {
             T s = (T) sqrt(trace + One<T>());
             w = s * Half<T>();

             T t = Half<T>() / s;
             x = (mat.m21 - mat.m12) * t;
             y = (mat.m02 - mat.m20) * t;
             z = (mat.m10 - mat.m01) * t;
           }
           else
           {
             //diagonal is negative
             int i = 0;
             if ( mat.m11 > mat.m00 ) i = 1;
             if ( mat.m22 > mat.m11 ) i = 2;

             static const int NEXT[3] = {1, 2, 0};
             int j = NEXT[i];
             int k = NEXT[j];

             T s = (T) sqrt( mat.GetRow(i)[i] - mat.GetRow(j)[j] + mat.GetRow(k)[k] + One<T>());

             (*this)[i] = s * Half<T>();

             T t;
             if(s != Zero<T>())   t = Half<T>() / s;
             else                 t = s;

             w          = (mat.GetRow(k)[j] - mat.GetRow(j)[k]) * t;
             (*this)[j] = (mat.GetRow(j)[i] - mat.GetRow(i)[j]) * t;
             (*this)[k] = (mat.GetRow(k)[i] - mat.GetRow(i)[k]) * t;
           }
         };
	inline ~Quaternion() {};

	static inline Quaternion<T> ShortestArc( const Vector3<T>& from, const Vector3<T>& to )
	{
	  assert(abs(from.SquaredLength() - One<T>()) < Epsilon<T>() && abs(from.SquaredLength() - One<T>()) < Epsilon<T>() && "shortestArc from i to cal que siguin unitaris");


		Vector3<T> cross = from ^ to; //Compute vector cross product
		T dot = from * to ;      //Compute dot product

		dot = (T) sqrt( 2*(dot+1) ) ; //We will use this equation twice

		cross /= dot ; //Get the x, y, z components

		//Return with the w component (Note that w is inverted because of left-handed rotations )
		return Quaternion<T>( cross[0], cross[1], cross[2], dot/2 ).GetNormalized() ;

	}

	static inline Quaternion<T> GetQuaternionFromRadians(Vector3<T> yawPitchRollInRads)
	{
		Vector3<T> euler = Vector3<T>( yawPitchRollInRads.z, yawPitchRollInRads.y, yawPitchRollInRads.x );
		float c1 = std::cos(euler.x * 0.5);
		float c2 = std::cos(euler.y * 0.5);
		float c3 = std::cos(euler.z * 0.5);
		float s1 = std::sin(euler.x * 0.5);
		float s2 = std::sin(euler.y * 0.5);
		float s3 = std::sin(euler.z * 0.5);
		float e = -1;

		Vector4<T> result;
		result.w = c1*c2*c3 - e*s1*s2*s3; // p0
		result.z = s1*c2*c3 + e*c1*s2*s3; // p1
		result.x = c1*s2*c3 - e*s1*c2*s3; // p2
		result.y = c1*c2*s3 + e*s1*s2*c3; // p3

		return  Quaternion<T>(result.x, result.y, result.z, result.w);
	}

	static inline Vector3<T> GetEulerFromQuaternion( Quaternion<T> q )
	{
		Vector3<double> euler;

		double p0 = q.w;
		double p1 = q.z;
		double p2 = q.x;
		double p3 = q.y;
		double e = -1;

		euler.y = std::asin(std::fmin(std::fmax(2 * ( p0*p2 + e*p1*p3 ), -1.), 1.));

		if(std::fabsf(euler.y-M_PI/2) < 0.05 )
		{
			euler.z = 0;
			euler.x = 2 * std::atan2( p1, p0 );
		}
		else if(std::fabsf(euler.y+M_PI/2) < 0.05 )
		{
			euler.z = 0;
			euler.x = - 2 * std::atan2( p1, p0 );
		}
		else
		{
			euler.x = std::atan2( 2 * ( p0*p1 - e*p2*p3 ), 1 - 2 * ( p1*p1 + p2*p2 ) );
			euler.z = std::atan2( 2 * ( p0*p3 - e*p1*p2 ), 1 - 2 * ( p2*p2 + p3*p3 ) );
		}

		return Vector3<T>(euler.z, euler.y, euler.x);
	}

	// member functions
public:
	inline T& operator[](unsigned int index)
	{
		return (&x)[index];
	}

	inline const T& operator[](unsigned int index) const
	{
		return (&x)[index];
	}

	inline void operator=(const Quaternion<T>& q)
		{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}

	inline void operator*=(const Quaternion<T>& q)
	{
		T qx, qy, qz, qw;
		qx = x;
		qy = y;
		qz = z;
		qw = w;

		x = qw * q.x + qx * q.w + qy * q.z - qz * q.y;
		y = qw * q.y - qx * q.z + qy * q.w + qz * q.x;
		z = qw * q.z + qx * q.y - qy * q.x + qz * q.w;
		w = qw * q.w - qx * q.x - qy * q.y - qz * q.z;
	}

	inline void operator*=(const Vector3<T>& v)
	{
		T qx, qy, qz, qw;
		qx = x;
		qy = y;
		qz = z;
		qw = w;

		x = qw * v.x            + qy * v.z - qz * v.y;
		y = qw * v.y - qx * v.z            + qz * v.x;
		z = qw * v.z + qx * v.y - qy * v.x;
		w =          - qx * v.x - qy * v.y - qz * v.z;
	}

  inline bool operator==(const Quaternion<T>& rhs) const
  {
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z &&
           w == rhs.w;
  }

  inline bool operator!=(const Quaternion<T>& rhs) const
  {
    return !operator==(rhs);
  }

  inline Quaternion<T> operator *(T f) const
  {
    return Quaternion(x * f, y * f, z * f, w * f);
  }

  inline Quaternion<T> operator /(T d) const
  {
    T f = One<T>() / d;
    return Quaternion(x * f, y * f, z * f, w * f);
  }

	inline void SLerp(T d, const Quaternion<T>& q)
	{
		T norm;
		norm = x * q.x + y * q.y + z * q.z + w * q.w;

		bool bFlip;
		bFlip = false;

		if(norm < Zero<T>())
		{
			norm = -norm;
			bFlip = true;
		}

		T inv_d;
		if(One<T>() - norm < Epsilon<T>())
		{
			inv_d = One<T>() - d;
		}
		else
		{
			T theta;
			theta = (T) acos(norm);

			T s;
			s = (T) (One<T>() / sin(theta));

			inv_d = (T) sin((One<T>() - d) * theta) * s;
			d = (T) sin(d * theta) * s;
		}

		if(bFlip)
		{
			d = -d;
		}

		x = inv_d * x + d * q.x;
		y = inv_d * y + d * q.y;
		z = inv_d * z + d * q.z;
		w = inv_d * w + d * q.w;
	}

	inline void Lerp(T d, const Quaternion<T>& q)
	{
		Vector4<T> first(w, y, z, w);
		Vector4<T> second(q.w, q.y, q.z, q.w);

		first.Lerp(second, d);

		return first.Normalize();
	}

	inline void Clear()
	{
		x = Zero<T>();
		y = Zero<T>();
		z = Zero<T>();
		w = One<T>();
	}

	inline void Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	inline Quaternion<T> GetConjugate() const
	{
		return Quaternion<T>(-x, -y, -z, w);
	}

	inline void Invert()
	{
		Conjugate();
		const T norm = (x*x) + (y*y) + (z*z) + (w*w);

		if (norm == Zero<T>()) return;

		const float inv_norm = One<T>() / norm;
		x *= inv_norm;
		y *= inv_norm;
		z *= inv_norm;
		w *= inv_norm;
	}

	inline Quaternion<T> GetInverted() const
	{
		Quaternion<T> q = GetConjugate();
		const T norm = (x*x) + (y*y) + (z*z) + (w*w);

		if (norm == Zero<T>()) return *this;

		const float inv_norm = One<T>() / norm;
		q.x *= inv_norm;
		q.y *= inv_norm;
		q.z *= inv_norm;
		q.w *= inv_norm;

    return q;
	}

	inline void Normalize()
	{
		const T norm = (x*x) + (y*y) + (z*z) + (w*w);

		if (norm == Zero<T>())
    {
      Clear();
    };

		const float inv_norm = One<T>() / norm;
		x *= inv_norm;
		y *= inv_norm;
		z *= inv_norm;
		w *= inv_norm;
	}

	inline Quaternion<T> GetNormalized() const
	{
		const T norm = (x*x) + (y*y) + (z*z) + (w*w);

		if (norm == Zero<T>())
    {
      return Quaternion<T>();
    };

		const float inv_norm = One<T>() / norm;
    return Quaternion<T>(
		                      x * inv_norm,
		                      y * inv_norm,
		                      z * inv_norm,
		                      w * inv_norm
                        );
	}

	inline void Set(T qx, T qy, T qz, T qw)
	{
		x = qx;
		y = qy;
		z = qz;
		w = qw;
	}


  inline void GetAxisAngle(Vector3<T>& axis, float& angle)
  {
    angle = 2 * acos(w);
    if(angle != 0)
    {
      axis = Vector3<T>(x, y, z) / sin(angle * Half<T>());
    }
    else
    {
      axis = Vector3<T>(0,0,0);
    }
  }
};

template<typename T>
static inline Quaternion<T> operator*(const Quaternion<T>& q, const Quaternion<T>& r)
{
	return Quaternion<T>(
		r.w * q.x + r.x * q.w + r.y * q.z - r.z * q.y,
		r.w * q.y - r.x * q.z + r.y * q.w + r.z * q.x,
		r.w * q.z + r.x * q.y - r.y * q.x + r.z * q.w,
		r.w * q.w - r.x * q.x - r.y * q.y - r.z * q.z
		);
}

template<typename T>
static inline Quaternion<T> operator+(const Quaternion<T>& q, const Quaternion<T>& r)
{
	return Quaternion<T>(q.x + r.x, q.y + r.y, q.z + r.z, q.w + r.w);
}


#endif

//****************************************************************************//
