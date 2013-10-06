/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_SPATIAL_HASHTABLE_H__
#define __PU_SPATIAL_HASHTABLE_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse
{
	/**	The SpatialHashTable is used to determine particles that are 'near' each other and discard particles that are further away. This
	    way only a few particles are taken into account for certain calculations.
	*/
	template <typename T>
	class _ParticleUniverseExport SpatialHashTable
	{
		public:
			typedef std::vector<T> HashTableCell;
			typedef std::vector<HashTableCell> HashTableContent;

			/**	Todo.
			*/
			SpatialHashTable(void) :
				mTableSize(50),
				mCellDimension(15),
				mCellOverlap(5)
			{
				_initialise();
			};

			/**	Todo.
			*/
			SpatialHashTable(unsigned int tableSize, unsigned short cellDimension, unsigned short cellOverlap = 0.0f) :
				mTableSize(tableSize),
				mCellDimension(cellDimension),
				mCellOverlap(cellOverlap)
			{
				_initialise();
			};

			/**	Destructor.
			*/
			virtual ~SpatialHashTable(void) {};

			/**	Clearing the hashtable means that every cell must be cleared, but not mHashTable itself. It still needs
				mTableSize vectors. Clearing mHashTable would implicate that all cells are removed, so you have to
				initialise again.
			*/
			void clear(void)
			{
				for (unsigned int i = 0; i < mTableSize; i++)
				{
					mHashTable[i].clear();
				}
			};

			/**	Todo.
			*/
			unsigned int size(void) const
			{
				return mTableSize;
			};

			/**	Todo.
			*/
			inline unsigned int put(const Vector3& position, T object)
			{
				// Put the object in the table, based on the position
				unsigned int cellIndexPosition = _calculateIndex(position);
				put(cellIndexPosition, object);

				/** If cell overlap is defined, take that into account. This means that the same object can be put 
					into multiple cells.
				*/
				if (mCellOverlap > 0.0f)
				{
					Vector3 corner;
					corner.x = position.x + mCellOverlap;
					corner.y = position.y + mCellOverlap;
					corner.z = position.z + mCellOverlap;
					unsigned int cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}

					corner.x = position.x + mCellOverlap;
					corner.y = position.y + mCellOverlap;
					corner.z = position.z - mCellOverlap;
					cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}

					corner.x = position.x + mCellOverlap;
					corner.y = position.y - mCellOverlap;
					corner.z = position.z + mCellOverlap;
					cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}

					corner.x = position.x - mCellOverlap;
					corner.y = position.y + mCellOverlap;
					corner.z = position.z + mCellOverlap;
					cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}

					corner.x = position.x + mCellOverlap;
					corner.y = position.y - mCellOverlap;
					corner.z = position.z - mCellOverlap;
					cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}

					corner.x = position.x - mCellOverlap;
					corner.y = position.y - mCellOverlap;
					corner.z = position.z + mCellOverlap;
					cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}

					corner.x = position.x - mCellOverlap;
					corner.y = position.y + mCellOverlap;
					corner.z = position.z - mCellOverlap;
					cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}

					corner.x = position.x - mCellOverlap;
					corner.y = position.y - mCellOverlap;
					corner.z = position.z - mCellOverlap;
					cellIndexCorner = _calculateIndex(corner);
					if (cellIndexPosition != cellIndexCorner)
					{
						put(cellIndexCorner, object);
					}
				}
				return cellIndexPosition;
			};

			/**	If you have multiple objects for which you already know the cellIndex, you can use this convenience 
				function to save time.
			*/
			inline unsigned int put(const unsigned int cellIndex, T object)
			{
				HashTableCell* cell = &(mHashTable[cellIndex]);
				cell->push_back(object);
				return cellIndex;
			};

			/**	Todo.
			*/
			inline HashTableCell& getCell(const Vector3& position)
			{
				return (mHashTable[_calculateIndex(position)]);
			};

			/**	Todo.
			*/
			inline unsigned int getCellIndex(const Vector3& position)
			{
				return _calculateIndex(position);
			};

		protected:
			typename SpatialHashTable::HashTableContent mHashTable;
			unsigned short mCellDimension; // Size of the cell
			unsigned short mCellOverlap; // Used to put nearby objects in a cel.
			unsigned int mTableSize;

			/**	Todo.
			*/
			inline unsigned int _calculateIndex(const Vector3& position)
			{
				long x = (long)((floor (position.x / mCellDimension)) * 73856093);
				long y = (long)((floor (position.y / mCellDimension)) * 19349663);
				long z = (long)((floor (position.z / mCellDimension)) * 83492791);
				long i = (x ^ y ^ z) % mTableSize;
				i = i < 0 ? mTableSize - 1 : i; // If negative, point to the last cell.
				return static_cast<unsigned int>(i);
			};

			/**	Todo.
			*/
			inline void _initialise(void)
			{
				mHashTable.clear();
				for (unsigned int i = 0; i < mTableSize; i++)
				{
					HashTableCell cell;
					mHashTable.push_back(cell);
				}
			}
	};

}
#endif
