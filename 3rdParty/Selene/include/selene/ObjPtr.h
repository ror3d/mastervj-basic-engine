#pragma once
#include <type_traits>

namespace sel
{
	template <typename T>
	class ObjPtr
	{
		T* _ptr;
		bool _managed;
	public:
		ObjPtr( T* ptr )
			: _ptr( ptr )
			, _managed( false )
		{
		}

		ObjPtr( T* ptr, bool managed )
			: _ptr( ptr )
			, _managed( managed )
		{
		}

		ObjPtr( ObjPtr<T>&& obj )
			: _ptr( obj._ptr )
			, _managed( obj._managed )
		{
			obj._ptr = nullptr;
			obj._managed = false;
		}

		void release()
		{
			if ( _managed )
			{
				delete _ptr;
				_managed = false;
			}
			_ptr = nullptr;
		}

		T* operator->() const
		{
			return _ptr;
		}

		T* get() const
		{
			return _ptr;
		}
	};
}
