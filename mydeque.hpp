#ifndef _MYDEQUE_HPP_
#define _MYDEQUE_HPP_

/************************************************************************************/

#include "messages.hpp"

#include <type_traits>
#include <cassert>
#include <iostream>
#include <cstddef>
#include <iostream>
#include <iterator>

/************************************************************************************/

struct Cell
{
	int m_blockIndex;

	int m_elementIndex;
};

struct AllocatedBlocksIndexes
{
	int m_frontIndex;

	int m_backIndex;
};

/************************************************************************************/

template< typename T >
class MyDeque
{
	/*------------------------------------------------------------------------------*/

	const int m_blockSize{ 10 };

	int m_directorySize;

	int m_frontIndex, m_backIndex;

	int m_frontBlockIndex, m_backBlockIndex;

	unsigned char ** m_data;

	enum actions { REDUCE = -1, INCREASE = 1 };

	/*------------------------------------------------------------------------------*/

	template< typename U >
	void copyElements( const MyDeque< U > & _deque );

	void grow();

	void resetData();

	void resetIndexes() noexcept;

	void deleteAllElements();

	void freeMemoryBlocks();

	bool isValidIndex( int _index ) const noexcept;

	Cell nextIndex( Cell _indexes, int _action ) const noexcept;

	Cell previousIndex( Cell _indexes, int _action ) const noexcept;

	T * getBlock( int _blockIndex ) const noexcept;

	void deleteElement( T * _pElement ) noexcept;

	unsigned char ** allocateDirectory() const;

	Cell calculateIndex( int _index ) const noexcept;

	void allocateMemory();

	void allocateBlock( int _blockIndex );

	AllocatedBlocksIndexes getAllocatedBlocksIndexes() const;

	/*------------------------------------------------------------------------------*/

public:

	/*------------------------------------------------------------------------------*/

	template< typename > friend class MyDeque;

	MyDeque();

	explicit MyDeque( int _elementsEmount, T _element = T() );

	template< typename U > MyDeque( std::initializer_list< U > _list );

	template< typename U > MyDeque( const MyDeque< U > & _deque );

	MyDeque( const MyDeque< T > & _deque );

	MyDeque( MyDeque< T > && _deque ) noexcept;

	~MyDeque();

	/*------------------------------------------------------------------------------*/

	template< typename U > MyDeque< T > & operator = ( const MyDeque< U > & _deque );

	MyDeque< T > & operator = ( const MyDeque< T > & _deque );

	MyDeque< T > & operator = ( MyDeque< T > && _deque ) noexcept;

	bool operator == ( const MyDeque< T > & _deque ) const noexcept;

	bool operator != ( const MyDeque< T > & _deque ) const noexcept;

	/*------------------------------------------------------------------------------*/

	const T & operator [] ( int _index ) const;

	T & operator [] ( int _index );

	const T & at( int _index ) const;

	T & at( int _index );

	const T & front() const;

	T & front();

	const T & back() const;

	T & back();

	/*------------------------------------------------------------------------------*/

	void push_back( const T & _element );

	void pop_back();

	void push_front( const T & _element );

	void pop_front();

	/*------------------------------------------------------------------------------*/

	void shrink_to_fit();

	int capacity() const noexcept;

	std::size_t size() const noexcept;

	bool empty() const noexcept;

	void clear();

	/*------------------------------------------------------------------------------*/

	template < class Deque >
	class Iterator
	{
		/*--------------------------------------------------------------------------*/

		int m_currentPosition;

		Deque & m_deque;

		/*--------------------------------------------------------------------------*/

	public:

		Iterator( Deque & _deque, unsigned int _currentPositon ) noexcept;

		Iterator( const Iterator<Deque> & _it ) noexcept;

		int getCurrentPosition() const noexcept;

		/*--------------------------------------------------------------------------*/

		bool operator == ( const Iterator<Deque> & _it ) const noexcept;

		bool operator != ( const Iterator<Deque> & _it ) const noexcept;

		bool operator >= ( const Iterator<Deque> & _it ) const noexcept;

		bool operator <= ( const Iterator<Deque> & _it ) const noexcept;

		bool operator > ( const Iterator<Deque> & _it ) const noexcept;

		bool operator < ( const Iterator<Deque> & _it ) const noexcept;

		/*--------------------------------------------------------------------------*/

		Iterator<Deque> operator + ( int _step ) const noexcept;

		Iterator<Deque> operator - ( int _step ) const noexcept;

		Iterator<Deque> & operator -= ( int _step ) noexcept;

		Iterator<Deque> & operator += ( int _step ) noexcept;

		/*--------------------------------------------------------------------------*/

		Iterator<Deque> operator + ( const Iterator<Deque> & _it ) const noexcept;

		Iterator<Deque> operator - ( const Iterator<Deque> & _it ) const noexcept;

		Iterator<Deque> & operator = ( const Iterator<Deque> & _it );

		Iterator<Deque> & operator -= ( const Iterator<Deque> & _it );

		Iterator<Deque> & operator += ( const Iterator<Deque> & _it );

		/*--------------------------------------------------------------------------*/

		Iterator<Deque> & operator ++ () noexcept;

		Iterator<Deque> operator ++ ( int ) noexcept;

		Iterator<Deque> & operator -- () noexcept;

		Iterator<Deque> operator -- ( int ) noexcept;

		/*--------------------------------------------------------------------------*/

		const T & operator *() const;

		template< typename = std::enable_if< !( std::is_const< Deque >::value ) >::type >
		T & operator *();
	};

	/*------------------------------------------------------------------------------*/

	using const_iterator = Iterator< const MyDeque< T > >;

	using iterator = Iterator< MyDeque< T > >;

	void erase( iterator _it );

	void erase( iterator _itBegin, iterator _itEnd );

	void insert( iterator _it, const T & _element );

	template< typename InputIt >
	void insert( InputIt _itBegin, InputIt _itEnd, iterator _itWhere);

	iterator begin() noexcept;

	iterator end() noexcept;

	const_iterator cbegin() const noexcept;

	const_iterator cend() const noexcept;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
MyDeque< T >::MyDeque()
{
	resetIndexes();

	m_data = allocateDirectory();
}

/**************************************************************************************************************/

template<typename T>
MyDeque< T >::MyDeque( int _elementsEmount, T _element )
	:	MyDeque()
{
	for ( int i = 0; i < _elementsEmount; i++ )
		push_back( _element );
}

/**************************************************************************************************************/

template< typename T >
template< typename U >
MyDeque< T >::MyDeque( std::initializer_list< U > _list )
	:	MyDeque()
{
	for ( const U & x : _list )
		this->push_back( x );
}

/**************************************************************************************************************/

template< typename T >
template< typename U >
MyDeque< T >::MyDeque( const MyDeque< U > & _deque )
	:	MyDeque()
{
	copyElements( _deque );
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T >::MyDeque( const MyDeque< T > & _deque )
	:	MyDeque()
{
	copyElements( _deque );
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T >::MyDeque( MyDeque< T > && _deque ) noexcept
	:	m_directorySize( _deque.m_directorySize )
	,	m_backIndex( _deque.m_backIndex )
	,	m_frontIndex( _deque.m_frontIndex )
	,	m_backBlockIndex( _deque.m_backBlockIndex )
	,	m_frontBlockIndex( _deque.m_frontBlockIndex )
{
	m_data = _deque.m_data;

	_deque.m_data = nullptr;

	_deque.resetIndexes();
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T >::~MyDeque()
{
	resetData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename T >
template< typename U >
MyDeque< T > & MyDeque< T >::operator = ( const MyDeque< U > & _deque )
{
	if ( ( const void * )( this ) == ( const void * )( &_deque ) )
		return *this;

	clear();

	m_data = allocateDirectory();

	copyElements( _deque );

	return *this;
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T > & MyDeque< T >::operator = ( const MyDeque< T > & _deque )
{
	if ( this == &_deque )
		return *this;

	clear();

	m_data = allocateDirectory();

	copyElements( _deque );

	return *this;
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T > & MyDeque< T >::operator = ( MyDeque< T > && _deque ) noexcept
{
	if ( this == &_deque )
		return *this;

	std::swap( m_directorySize, _deque.m_directorySize );
	std::swap( m_frontIndex, _deque.m_frontIndex );
	std::swap( m_backIndex, _deque.m_backIndex );
	std::swap( m_frontBlockIndex, _deque.m_frontBlockIndex );
	std::swap( m_backBlockIndex, _deque.m_backBlockIndex );
	std::swap( m_data, _deque.m_data );
}

/**************************************************************************************************************/

template< typename T >
bool MyDeque< T >::operator == ( const MyDeque< T > & _deque ) const noexcept
{
	int size = this->size();

	if ( size != _deque.size() )
		return false;

	for ( int i = 0; i < size; i++ )
		if ( ( *this )[i] != _deque[i] )
			return false;

	return true;
}

/**************************************************************************************************************/

template< typename T >
bool MyDeque< T >::operator != ( const MyDeque< T > & _deque ) const noexcept
{
	return !( *this == _deque );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::push_back( const T & _element )
{
	allocateMemory();

	new ( getBlock( m_backBlockIndex ) + m_backIndex ) T( _element );

	Cell newIndexes = nextIndex( Cell{ m_backBlockIndex, m_backIndex }, actions::INCREASE );
	m_backIndex = newIndexes.m_elementIndex;
	m_backBlockIndex = newIndexes.m_blockIndex;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::pop_back()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	Cell newIndexes = previousIndex( Cell{ m_backBlockIndex, m_backIndex }, actions::REDUCE );
	m_backIndex = newIndexes.m_elementIndex;
	m_backBlockIndex = newIndexes.m_blockIndex;

	deleteElement( getBlock( m_backBlockIndex ) + m_backIndex );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::push_front( const T & _element )
{
	allocateMemory();

	new ( getBlock( m_frontBlockIndex ) + m_frontIndex ) T( _element );

	Cell newIndexes = previousIndex( Cell{ m_frontBlockIndex, m_frontIndex }, actions::REDUCE );
	m_frontIndex = newIndexes.m_elementIndex;
	m_frontBlockIndex = newIndexes.m_blockIndex;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::pop_front()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	Cell newIndexes = nextIndex( Cell{ m_frontBlockIndex, m_frontIndex }, actions::INCREASE );
	m_frontBlockIndex = newIndexes.m_blockIndex;
	m_frontIndex = newIndexes.m_elementIndex;

	deleteElement( getBlock( m_frontBlockIndex ) + m_frontIndex );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::clear()
{
	if ( empty() )
		return;

	resetData();

	resetIndexes();

	m_data = allocateDirectory();
}

/**************************************************************************************************************/

template< typename T >
bool MyDeque< T >::empty() const noexcept
{
	return size() == 0;
}

/**************************************************************************************************************/

template< typename T >
std::size_t MyDeque< T >::size() const noexcept
{
	if( m_frontBlockIndex == m_backBlockIndex )
	{
		if( m_backIndex - 1 == m_frontIndex )
			return 0;
		
		return m_backIndex - m_frontIndex - 1; // -1 because indexes are set for writing and not pointing on current element in block
	}

	size_t size = 0;
	size += m_blockSize - m_frontIndex - 1; // -1 because frontIndex pointing on index for writing, but not on real element
	size += ( m_backBlockIndex - m_frontBlockIndex - 1 ) * m_blockSize;
	size += m_backIndex;
	return size;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::front() const
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	Cell resultIndexes = nextIndex( Cell{  m_frontBlockIndex, m_frontIndex }, actions::INCREASE );
	T * result = getBlock( resultIndexes.m_blockIndex ) + resultIndexes.m_elementIndex;

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::back() const
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	Cell resultIndexes = previousIndex( Cell{ m_backBlockIndex, m_backIndex }, actions::REDUCE );
	T * result = getBlock( resultIndexes.m_blockIndex ) + resultIndexes.m_elementIndex;

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::front()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	Cell resultIndexes = nextIndex( Cell{ m_frontBlockIndex, m_frontIndex }, actions::INCREASE );
	T * result = getBlock( resultIndexes.m_blockIndex ) + resultIndexes.m_elementIndex;

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::back()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	Cell resultIndexes = previousIndex( Cell{ m_backBlockIndex, m_backIndex }, actions::REDUCE );
	T * result = getBlock( resultIndexes.m_blockIndex ) + resultIndexes.m_elementIndex;

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::operator [] ( int _index ) const
{
	Cell resultIndexes = calculateIndex( _index );
	T * result = getBlock( resultIndexes.m_blockIndex ) + resultIndexes.m_elementIndex;

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::operator [] ( int _index )
{
	Cell resultIndexes = calculateIndex( _index );
	T * result = getBlock( resultIndexes.m_blockIndex ) + resultIndexes.m_elementIndex;

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::at( int _index ) const
{
	if ( !isValid( _index ) )
		throw std::logic_error( Messages::InvalidIndex );

	return ( *this )[_index];
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::at( int _index )
{
	if ( !isValidIndex( _index ) )
		throw std::logic_error( Messages::InvalidIndex );

	return ( *this )[_index];
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::grow()
{
	m_directorySize *= 2;
	unsigned char ** newDirectory = allocateDirectory();

	int delta = m_directorySize / 4; // we need a half 
	for( int i{ 0 }; i < m_directorySize / 2; i++ )
	{
		newDirectory[delta + i] = m_data[i];
	}

	m_frontBlockIndex = delta + m_frontBlockIndex;
	m_backBlockIndex = delta + m_backBlockIndex;

	delete[] m_data;
	m_data = newDirectory;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::shrink_to_fit()
{
	for( int i{ 0 }; i < m_frontBlockIndex; ++i )
		delete[] m_data[i];
	for( int i{ m_backBlockIndex + 1 }; i < m_directorySize; ++i )
		delete[] m_data[i];

	m_directorySize = m_backBlockIndex - m_frontBlockIndex + 1;
	int counter = m_frontBlockIndex;

	unsigned char ** shrinked_data = new unsigned char*[m_directorySize];
	for ( int i{ 0 }; i < m_directorySize; i++ )
	{
		shrinked_data[i] = m_data[counter];
		++counter;
	}

	m_frontBlockIndex = 0;
	m_backBlockIndex = m_directorySize - 1;

	delete[] m_data;
	m_data = shrinked_data;
}

/**************************************************************************************************************/

template<typename T>
int MyDeque< T >::capacity() const noexcept
{
	auto allocatedBlocks = getAllocatedBlocksIndexes();

	return ( allocatedBlocks.m_backIndex - allocatedBlocks.m_frontIndex + 1 ) * m_blockSize;
}

/**************************************************************************************************************/

template< typename T >
Cell MyDeque< T >::nextIndex( Cell _indexes, int _action ) const noexcept
{
	assert( _indexes.m_elementIndex >= 0 );
	assert( _indexes.m_elementIndex <= m_blockSize );

	if ( _indexes.m_elementIndex == m_blockSize - 1 )
	{
		_indexes.m_blockIndex += _action;
		_indexes.m_elementIndex = 0;
	}
	else
		++_indexes.m_elementIndex;

	return _indexes;
}

/**************************************************************************************************************/

template< typename T >
Cell MyDeque< T >::previousIndex( Cell _indexes, int _action ) const noexcept
{
	assert( _indexes.m_elementIndex >= 0 );
	assert( _indexes.m_elementIndex <= m_blockSize );

	if ( _indexes.m_elementIndex == 0 )
	{
		_indexes.m_blockIndex += _action;
		_indexes.m_elementIndex = m_blockSize - 1;
	}
	else
		--_indexes.m_elementIndex;

	return _indexes;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::resetData()
{
	deleteAllElements();

	freeMemoryBlocks();
}

/**************************************************************************************************************/

template<typename T>
void MyDeque< T >::resetIndexes() noexcept
{
	m_directorySize = 2;

	m_backIndex = 1;
	m_frontIndex = 0;

	m_frontBlockIndex = m_directorySize / 2;
	m_backBlockIndex = m_directorySize / 2;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::deleteAllElements()
{
	if constexpr ( std::is_class< T >::value )
	{
		for( auto & element : *this )
			element.~T();
	}
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::freeMemoryBlocks()
{
	if( !m_data )
		return;

	for( int i{ 0 }; i < m_directorySize; i++ )
		delete[] m_data[i];

	delete[] m_data;
}

/**************************************************************************************************************/

template<typename T>
inline bool MyDeque<T>::isValidIndex( int _index ) const noexcept
{
	return ( _index >= 0 ) && ( _index < size() );
}

/**************************************************************************************************************/

template< typename T >
template< typename U >
void MyDeque< T >::copyElements( const MyDeque< U > & _deque )
{
	int size = _deque.size();
	for ( int i = 0; i < size; i++ )
		push_back( _deque[i] );
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::iterator MyDeque< T >::begin() noexcept
{
	iterator it( *this, 0 );
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::iterator MyDeque< T >::end() noexcept
{
	iterator it( *this, size() );
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::const_iterator MyDeque< T >::cbegin() const noexcept
{
	const_iterator it( *this, 0 );
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::const_iterator MyDeque< T >::cend() const noexcept
{
	const_iterator it( *this, size() );
	return it;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::erase( iterator _it )
{
	if ( !isValidIndex( _it.getCurrentPosition() ) )
		throw std::logic_error( Messages::InvalidIterator );

	int size = this->size();

	MyDeque< T >::iterator itNext = _it;
	MyDeque< T >::iterator itCurrent = _it;

	if ( _it.getCurrentPosition() > size / 2 )
	{
		++itNext;

		for ( ; itNext.getCurrentPosition() < size; ++itNext, ++itCurrent )
			*itCurrent = std::move( *itNext );

		pop_back();
	}
	else
	{
		--itNext;

		for ( ; itNext.getCurrentPosition() > 0; --itNext, --itCurrent )
			*itCurrent = std::move( *itNext );

		pop_front();
	}
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::erase( iterator _itBegin, iterator _itEnd )
{
	if ( _itEnd.getCurrentPosition() < _itBegin.getCurrentPosition() ||
		!isValidIndex( _itBegin.getCurrentPosition() ) ||
		!isValidIndex( _itEnd.getCurrentPosition() ) )
		throw std::logic_error( Messages::InvalidIterator );

	int lastIndex = size() - 1;

	int copyFront = _itBegin.getCurrentPosition();
	int copyBack = lastIndex - _itEnd.getCurrentPosition();

	if ( copyBack > copyFront )
	{
		--_itBegin;

		for ( ; _itBegin.getCurrentPosition() >= 0; --_itBegin, --_itEnd )
			*_itEnd = std::move_if_noexcept( *_itBegin );

		int difference = _itEnd.getCurrentPosition() - _itBegin.getCurrentPosition();
		for ( int i{ 0 }; i < difference; ++i )
			pop_front();
	}
	else
	{
		++_itEnd;

		for ( ; _itEnd.getCurrentPosition() <= lastIndex; ++_itBegin, ++_itEnd )
			*_itBegin = std::move_if_noexcept( *_itEnd );

		int difference = _itEnd.getCurrentPosition() - _itBegin.getCurrentPosition();
		for ( int i{ 0 }; i < difference; ++i )
			pop_back();
	}
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::insert( iterator _it, const T & _element )
{
	if ( !isValidIndex( _it.getCurrentPosition() ) )
		throw std::logic_error( Messages::InvalidIterator );

	int size = this->size();

	if ( _it.getCurrentPosition() > size / 2 )
	{
		Cell newIndexes = nextIndex( Cell{ m_backBlockIndex, m_backIndex }, actions::INCREASE );
		m_backBlockIndex = newIndexes.m_blockIndex;
		m_backIndex = newIndexes.m_elementIndex;
		allocateMemory();

		iterator itCurrent = end() - 1;
		iterator itNext = itCurrent - 1;

		for ( ; itCurrent != _it; --itNext, --itCurrent )
			*itCurrent = std::move_if_noexcept( *itNext );
	}
	else
	{
		iterator itCurrent = begin();
		new( getBlock( m_frontBlockIndex ) + m_frontIndex ) T( std::move_if_noexcept( *itCurrent ) );

		iterator itNext = itCurrent + 1;

		for ( ; itCurrent != _it;  ++itNext, ++itCurrent )
			*itCurrent = std::move_if_noexcept( *itNext );

		Cell newIndexes = previousIndex( Cell{ m_frontBlockIndex, m_frontIndex }, actions::REDUCE );
		m_frontBlockIndex = newIndexes.m_blockIndex;
		m_frontIndex = newIndexes.m_elementIndex;
		allocateMemory();
	}

	*_it = _element;
}

/**************************************************************************************************************/

template< typename T >
template< class InputIt >
void MyDeque< T >::insert( InputIt _itBegin, InputIt _itEnd, iterator _itWhere )
{
	for ( ; _itBegin != _itEnd; ++_itBegin, ++_itWhere )
		insert( _itWhere, *_itBegin );
}

/**************************************************************************************************************/

template< typename T >
Cell MyDeque< T >::calculateIndex( int _index ) const noexcept
{
	Cell result;
	if ( _index < m_blockSize - ( m_frontIndex + 1 ) )
	{
		result.m_blockIndex = m_frontBlockIndex;
		result.m_elementIndex = ( m_frontIndex + 1 ) + _index;
	}
	else
	{
		_index -= m_blockSize - ( m_frontIndex + 1 );
		result.m_blockIndex = m_frontBlockIndex + _index / m_blockSize + 1;
		result.m_elementIndex = _index % m_blockSize;
	}
	return result;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::allocateMemory()
{
	if ( m_frontBlockIndex == -1 || m_backBlockIndex == m_directorySize )
		grow();

	if ( !m_data[m_frontBlockIndex] )
		allocateBlock( m_frontBlockIndex );

	if ( !m_data[m_backBlockIndex] )
		allocateBlock( m_backBlockIndex );
}

/**************************************************************************************************************/

template<typename T>
AllocatedBlocksIndexes MyDeque< T >::getAllocatedBlocksIndexes() const
{
	AllocatedBlocksIndexes result{ 0, -1 }; // -1 for correct working with empty deque

	bool foundFirstAllocatedBlock = false;
	for( int i{ 0 }; i < m_directorySize; ++i )
	{
		if( !foundFirstAllocatedBlock && m_data[i] )
		{
			foundFirstAllocatedBlock = true;
			result.m_frontIndex = i;
		}
		else if( foundFirstAllocatedBlock && m_data[i] )
		{
			result.m_backIndex = i;
		}
		else
			break;
	}

	return result;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::allocateBlock( int _blockIndex )
{
	m_data[_blockIndex] = new unsigned char[m_blockSize * sizeof( T )];
}

/**************************************************************************************************************/

template< typename T >
inline T * MyDeque< T >::getBlock( int _blockIndex ) const noexcept
{
	return reinterpret_cast< T* >( m_data[_blockIndex] );
}

/**************************************************************************************************************/

template<typename T>
inline void MyDeque< T >::deleteElement( T * _pElement ) noexcept
{
	if constexpr ( std::is_class< T >::value )
		_pElement->~T();
}

/**************************************************************************************************************/

template<typename T>
unsigned char ** MyDeque<T>::allocateDirectory() const
{
	unsigned char ** newDirectory = new unsigned char*[m_directorySize];

	for( int i{ 0 }; i < m_directorySize; ++i )
		newDirectory[i] = nullptr;

	return newDirectory;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque >::Iterator( Deque & _deque, unsigned int _currentPositon ) noexcept
	:m_deque( _deque ), m_currentPosition( _currentPositon )
{
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque >::Iterator( const Iterator<Deque> & _it ) noexcept
	:	m_deque( _it.m_deque )
	,	m_currentPosition( _it.m_currentPosition )
{
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator == ( const Iterator< Deque > & _it ) const noexcept
{
	bool result = ( m_currentPosition == _it.m_currentPosition ) &&
		( ( const void * )&m_deque == ( const void * )&_it.m_deque );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator != ( const Iterator< Deque > & _it ) const noexcept
{
	return !( *this == _it );
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator >= ( const Iterator< Deque > & _it ) const noexcept
{
	return m_currentPosition >= _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator <= ( const Iterator< Deque > & _it ) const noexcept
{
	return m_currentPosition <= _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator > ( const Iterator< Deque > & _it ) const noexcept
{
	return m_currentPosition > _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator < ( const Iterator< Deque > & _it ) const noexcept
{
	return m_currentPosition < _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template < class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator + ( int _step ) const noexcept
{
	//assert(m_currentPosition + _step < m_deque.size());

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition + _step );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator - ( int _step ) const noexcept
{
	//assert(m_currentPosition - _step >= 0);

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition - _step );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator + ( const Iterator< Deque > & _it ) const noexcept
{
	//assert( (m_currentPosition + _it.m_currentPosition) < m_deque.size());

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition + _it.m_currentPosition );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator - ( const Iterator< Deque > & _it ) const noexcept
{
	//assert(m_currentPosition - _it.m_currentPosition >= 0);

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition - _it.m_currentPosition );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator = ( const Iterator< Deque > & _it )
{
	m_currentPosition = _it.m_currentPosition;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator -= ( const Iterator< Deque > & _it )
{
	//assert(m_currentPosition - _it.m_currentPosition() >= 0);

	m_currentPosition -= _it.m_currentPosition;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator += ( const Iterator< Deque > & _it )
{
	m_currentPosition += _it.m_currentPosition;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator -= ( int _step ) noexcept
{
	m_currentPosition -= _step;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator += ( int _step ) noexcept
{
	m_currentPosition += _step;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator ++ () noexcept
{
	//assert(m_currentPosition < m_deque.size());

	++m_currentPosition;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator ++ ( int ) noexcept
{
	//assert(m_currentPosition < m_deque.size());

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition );
	++m_currentPosition;

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator -- () noexcept
{
	//assert(m_currentPosition > 0);

	--m_currentPosition;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator -- ( int ) noexcept
{
	//assert(m_currentPosition > 0);

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition );
	--m_currentPosition;

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
const T & MyDeque< T >::Iterator< Deque >::operator *() const
{
	return m_deque[m_currentPosition];
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
template< typename = std::enable_if< !( std::is_const< Deque >::value ) >::type >
T & MyDeque< T >::Iterator< Deque >::operator *()
{
	return m_deque[m_currentPosition];
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
int MyDeque< T >::Iterator< Deque >::getCurrentPosition() const noexcept
{
	return m_currentPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // !_MYDEQUE_HPP_