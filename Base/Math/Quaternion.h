#pragma once

#ifndef __QUATERNION__
#define __QUATERNION__

#include "MathTypes.h"
#include "MathUtils.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Utils/Types.h"


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

template<typename T>
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


#endif

//****************************************************************************//
