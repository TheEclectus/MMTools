#pragma once

#include <cstdlib>

#include "Error.h"
#include "Typedefs.h"

// TODO: See if a single Array class is possible.

namespace MMT
{
	// T must have a default constructor
	template<class T> class Array1d;
	template<class T> class Array1dRef;
	template<class T> class Array2d;
	template<class T> class Array2dRef;
	template<class T> class Array3d;

	/*
	REMEMBER:
	The ArrayXd classes are not reference-counted.
	The data inside exists for the lifetime of the object.
	References can be made to them for low-overhead data
	access, or can be copied completely.
	Don't be a dumbass. Manage your memory like you'd
	manage your money, and keep everything accounted for.
	*/

	template<class T, size_t Dimensions> class Array;
	template<class T, size_t Dimensions> class ArrayRef;

	template<class T, size_t Dimensions>
	class ArrayRef
	{
	protected:
		const Array<T, Dimensions> &_data;
	public:
		ArrayRef(const Array<T, Dimensions> &ParentData) :
			_data(ParentData)
		{

		}

		const Array<T, Dimensions> &Get() const
		{
			return _data;
		}
	};

	template<class T, size_t Dimensions>
	class Array
	{
	protected:
		T *_data;
		size_t _dimensionSize[Dimensions];
	public:
		// Format: {x, y, z}
		Array(const size_t DimensionSizes[Dimensions]) :
			_data(nullptr),
			_dimensionSize(DimensionSizes)
		{
			size_t ElementCount = 0;
			for (size_t i = 0; i < Dimensions; i++)
			{
				auto Cur = _dimensionSize[i];

				if (i == 0)
				{
					ElementCount = Cur;
				}
				else
				{
					ElementCount *= Cur;
				}
			}
			_data = new T[ElementCount];
		}

		const ArrayRef<T, Dimensions> GetRef() const
		{
			return ArrayRef<T, Dimensions>(*this);
		}

		Array<T, Dimensions - 1> operator[](size_t Index)
		{
			size_t Dims = Dimensions - 1;
			if (Dims == 0)
			{

			}
			else
			{

			}
		}
	};

	template<class T>
	class Array1d
	{
	protected:
		T *_data;
		u32 _width;
	public:
		Array1d() :
			_data(nullptr),
			_width(0u)
		{

		}

		Array1d(const u32 &Width) :
			_data(new T[Width]()),
			_width(Width)
		{

		}

		Array1d(T *Data, const u32 &Width) :
			_data(Data),
			_width(Width)
		{

		}

		//Array1d(const Array1d &ConstructionCopy) = delete;
		//Array1d &operator=(const Array1d &Copy) = delete;

		//operator Array1d() = delete;

		virtual ~Array1d()
		{
			delete[] _data;
		}

		T &operator[](const u32 &Index) const
		{
			//ErrorContext EC("While accessing MMT::Array1d");
			xassert(Index < _width, "Attempted to access out-of-bounds array index.\n\tAttempted index: %u\n\tArray size: %u", Index, _width);
			return _data[Index];
		}

		inline const u32 GetWidth() const
		{
			return _width;
		}
	};

	// Ref class can't modify the parent data
	template<class T>
	class Array1dRef
	{
		//friend class Array;
	private:
		//const Array1d<T> * const _parentData;
		T *_data;
		u32 _width;
	public:
		Array1dRef(T *Data, u32 Width) :
			_data(Data),
			_width(Width)
		{
			
		}

		inline T &operator[](const u32 &Index) const
		{
			return (_data[Index]);
		}

		inline const u32 GetWidth() const
		{
			return _parentData->GetWidth();
		}
	};

	// TODO: Make it possible to resize an array
	template<class T>
	class Array2d 
	{
	protected:
		T **_data;
		u32 _width, _height;
	public:
		Array2d() :
			_data(nullptr),
			_width(0u),
			_height(0u)
		{
			
		}

		Array2d(const u32 &Height, const u32 &Width) :
			_data(nullptr),
			_width(Width),
			_height(Height)
		{
			_data = new T*[_height];
			for (int i = 0; i < _height; i++)
			{
				_data[i] = new T[_width]();
			}
		}

		Array2d(T **Data, const u32 &Height, const u32 &Width) :
			_data(Data),
			_width(Width),
			_height(Height)
		{

		}

		//Array2d(const Array2d &ConstructionCopy) = delete;
		void operator=(Array2d &Copy)
		{
			_data = Copy._data;
			_width = Copy._width;
			_height = Copy._height;

			Copy._data = nullptr;
			Copy._width = 0;
			Copy._height = 0;
		}

		void Clear()
		{
			for (int i = 0; i < _height; i++)
			{
				memset(_data[i], 0, sizeof(T)*_width);
			}
		}

		virtual ~Array2d()
		{
			for (int i = 0; i < _height; i++)
			{
				delete[] _data[i];
			}
			delete[] _data;
		}

		operator Array2dRef<T>()
		{
			return Array2dRef<T>(_data, _height, _width);
		}

		void PrintPreview()
		{
			for (int y = 0; y < _height; y++)
			{
				for (int x = 0; x < _width; x++)
				{
					Tile &a = _data[y][x];
					fmt::printf("%c", (a.Character != 0 && a.Character != 32) ? a.Character : ' ');
				}
				fmt::printf("\n");
			}
		}

		void CopyToPosition(const Array2d<T> &From, const Point &Location)
		{
			for (int y = 0; y < From._height; y++)
			{
				for (int x = 0; x < From._width; x++)
				{
					if ((Location.X + x >= 0) && (Location.X + x < this->_width) && (Location.Y + y >= 0) && (Location.Y + y < this->_height))
					{
						_data[Location.Y + y][Location.X + x] = From._data[y][x];
					}
				}
			}
		}

		void CopyToPosition(const Array2dRef<T> &From, const Point &Location)
		{
			for (int y = 0; y < From._height; y++)
			{
				for (int x = 0; x < From._width; x++)
				{
					if ((Location.X + x >= 0) && (Location.X + x < this->_width) && (Location.Y + y >= 0) && (Location.Y + y < this->_height))
					{
						_data[Location.Y + y][Location.X + x] = From._data[y][x];
					}
				}
			}
		}

		Array1dRef<T> operator[](const u32 &Index) const
		{
			//ErrorContext EC("While accessing MMT::Array2d");
			xassert(Index < _height, "Attempted to access out-of-bounds array index.\n\tAttempted index: %u\n\tArray size: %u", Index, _height);
			return Array1dRef<T>(_data[Index], _width);
		}

		inline const u32 GetWidth() const
		{
			return _width;
		}

		inline const u32 GetHeight() const
		{
			return _height;
		}
	};

	template<class T>
	class Array2dRef
	{
		friend class Array2d<T>;
	private:
		T **_data;
		u32 _width, _height;
	public:
		Array2dRef(T **Data, u32 Height, u32 Width) :
			_data(Data),
			_width(Width),
			_height(Height)
		{

		}

		inline const Array1dRef<T> operator[](const u32 &Index) const
		{
			//return (*_parentData)[Index];
			return Array1dRef<T>(_data[Index], _width);
		}

		inline const u32 GetWidth() const
		{
			return _width;
		}

		inline const u32 GetHeight() const
		{
			return _height;
		}

		void PrintPreview()
		{
			for (int y = 0; y < _height; y++)
			{
				for (int x = 0; x < _width; x++)
				{
					MTile &a = _data[y][x];
					fmt::printf("%c", (a.character != 0 && a.character != 32) ? a.character : ' ');
				}
				fmt::printf("\n");
			}
		}
	};

	template<class T>
	class Array3d
	{
	protected:
		T ***_data;
		u32 _width, _height, _depth;
	public:
		Array3d() :
			_data(nullptr),
			_width(0u),
			_height(0u),
			_depth(0u)
		{

		}

		Array3d(const u32 &Depth, const u32 &Height, const u32 &Width) :
			_data(nullptr),
			_width(Width),
			_height(Height),
			_depth(Depth)
		{
			_data = new T**[_depth];
			for (int y = 0; y < _depth; y++)
			{
				_data[y] = new T*[_height];
				for (int x = 0; x < _height; x++)
				{
					_data[y][x] = new T[_width]();
				}
			}
		}

		virtual ~Array3d()
		{
			for (int y = 0; y < _depth; y++)
			{
				for (int x = 0; x < _height; x++)
				{
					delete[] _data[y][x];
				}
				delete[] _data[y];
			}
			delete[] _data;
		}

		//Array3d(const Array3d &ConstructionCopy) = delete;
		//Array3d &operator=(const Array3d &Copy) = delete;

		void Claim(Array3d &Assign)
		{
			_data = Assign._data;
			_width = Assign._width;
			_height = Assign._height;
			_depth = Assign._depth;

			//return *this;
		}

		Array2dRef<T> operator[](const u32 &Index) const
		{
			//ErrorContext EC("While accessing MMT::Array3d");
			xassert(Index < _depth, "Attempted to access out-of-bounds array index.\n\tAttempted index: %u\n\tArray size: %u", Index, _depth);
			return Array2dRef<T>(_data[Index], _height, _width);
		}

		inline const u32 GetWidth() const
		{
			return _width;
		}

		inline const u32 GetHeight() const
		{
			return _height;
		}

		inline const u32 GetDepth() const
		{
			return _depth;
		}
	};

	template<class T>
	class Array3dRef
	{
	private:
		//const Array3d<T> * const _parentData;
		T ***_data;
		u32 _width, _height, _depth;
	public:
		Array3dRef(T ***Data, u32 Width, u32 Height, u32 Depth) :
			_data(Data),
			_width(Width),
			_height(Height),
			_depth(Depth)
		{

		}

		inline const Array2dRef<T> &operator[](const u32 &Index) const
		{
			return Array2dRef<T>(_data[Index], _width, _height);
		}

		inline const u32 GetWidth() const
		{
			return _width;
		}

		inline const u32 GetHeight() const
		{
			return _height;
		}

		inline const u32 GetDepth() const
		{
			return _depth;
		}
	};

	//typedef Array2d<MTile> TileImage;
	typedef Array1d<Array2d<MMT::Tile>> TileImageStrip;
}