#ifndef MYDEQUE
#define MYDEQUE

/************************************************************************************/

#include "messages.hpp"

#include <type_traits>
#include <cassert>
#include <iostream>
#include <cstddef>
#include <iostream>
#include <iterator>

/************************************************************************************/

template< typename T >
class MyDeque
{
	/*----------------------------------------------------------------------------------*/

	const int m_blockSize{ 10 };

	int m_directorySize;

	int m_frontIndex, m_backIndex;

	int m_frontBlockIndex, m_backBlockIndex;

	int m_frontAllocatedBlockIndex, m_backAllocatedBlockIndex;

	unsigned char ** m_data;

	enum actions { REDUCE = -1, INCREASE = 1 };

	/*----------------------------------------------------------------------------------*/

	template< typename U >
	void copyElements( const MyDeque< U > & _deque );

	void grow();

	void resetData();

	void resetIndexes();

	void deleteAllElements();

	void freeMemoryBlocks();

	bool isValid( int _index ) const { return ( _index >= 0 ) && ( _index < size() ); }

	void nextIndex( int & _index, int & _blockIndex, int _action );

	void previousIndex( int & _index, int & _blockIndex, int _action );

	T * getElement( unsigned char * _pElement ) const;

	void deleteElement( unsigned char * _pElement );

	void allocateDirectory();

	std::pair<int, int> calculateIndex( int _index ) const; // first => blockIndex, second => elementIndex

	void allocateMemory();

	void allocateBlock( int & _blockIndex, int & _allocatedBlockIndex );

	/*----------------------------------------------------------------------------------*/

public:

	/*----------------------------------------------------------------------------------*/

	template< typename > friend class MyDeque;

	MyDeque();

	MyDeque( int _elementsEmount, T _element = T() );

	template< typename U > MyDeque( std::initializer_list< U > _list );

	template< typename U > MyDeque( const MyDeque< U > & _deque );

	MyDeque( const MyDeque< T > & _deque );

	MyDeque( MyDeque< T > && _deque );

	~MyDeque();

	/*----------------------------------------------------------------------------------*/

	template< typename U > MyDeque< T > & operator = ( const MyDeque< U > & _deque );

	MyDeque< T > & operator = ( const MyDeque< T > & _deque );

	MyDeque< T > & operator = ( MyDeque< T > && _deque );

	bool operator == ( const MyDeque< T > & _deque ) const;

	bool operator != ( const MyDeque< T > & _deque ) const;

	/*----------------------------------------------------------------------------------*/

	const T & operator [] ( int _index ) const;

	T & operator [] ( int _index );

	const T & at( int _index ) const;

	T & at( int _index );

	const T & front() const;

	T & front();

	const T & back() const;

	T & back();

	/*----------------------------------------------------------------------------------*/

	void push_back( const T & _element );

	void pop_back();

	void push_front( const T & _element );

	void pop_front();

	/*----------------------------------------------------------------------------------*/

	void shrink_to_fit();

	std::size_t size() const;

	bool empty() const;

	void clear();

	/*----------------------------------------------------------------------------------*/

	template < class Deque >
	class Iterator
	{
		/*----------------------------------------------------------------------------------*/

		int m_currentPosition;

		Deque & m_deque;

		/*----------------------------------------------------------------------------------*/

	public:

		Iterator( Deque & _deque, unsigned int _currentPositon );

		Iterator( const Iterator<Deque> & _it );

		int getCurrentPosition() const;

		/*----------------------------------------------------------------------------------*/

		bool operator == ( const Iterator<Deque> & _it ) const;

		bool operator != ( const Iterator<Deque> & _it ) const;

		bool operator >= ( const Iterator<Deque> & _it ) const;

		bool operator <= ( const Iterator<Deque> & _it ) const;

		bool operator > ( const Iterator<Deque> & _it ) const;

		bool operator < ( const Iterator<Deque> & _it ) const;

		/*----------------------------------------------------------------------------------*/

		Iterator<Deque> operator + ( int _step ) const;

		Iterator<Deque> operator - ( int _step ) const;

		Iterator<Deque> & operator -= ( int _step );

		Iterator<Deque> & operator += ( int _step );

		/*----------------------------------------------------------------------------------*/

		Iterator<Deque> operator + ( const Iterator<Deque> & _it ) const;

		Iterator<Deque> operator - ( const Iterator<Deque> & _it ) const;

		Iterator<Deque> & operator = ( const Iterator<Deque> & _it );

		Iterator<Deque> & operator -= ( const Iterator<Deque> & _it );

		Iterator<Deque> & operator += ( const Iterator<Deque> & _it );

		/*----------------------------------------------------------------------------------*/

		Iterator<Deque> & operator ++ ();

		Iterator<Deque> operator ++ ( int );

		Iterator<Deque> & operator -- ();

		Iterator<Deque> operator -- ( int );

		/*----------------------------------------------------------------------------------*/

		const T & operator *() const;

		template< typename = std::enable_if< !( std::is_const< Deque >::value ) >::type >
		T & operator *();
	};

	/*----------------------------------------------------------------------------------*/

	using const_iterator = Iterator< const MyDeque< T > >;

	using iterator = Iterator< MyDeque< T > >;

	void erase( iterator _it );

	void erase( iterator _itBegin, iterator _itEnd ); // choose side

	void insert( iterator _it, const T & _element ); // choose side

	template< typename InputIt >
	void insert( InputIt _itBegin, InputIt _itEnd, iterator _itWhere);

	iterator begin();

	iterator end();

	const_iterator cbegin() const;

	const_iterator cend() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
MyDeque< T >::MyDeque()
{
	resetIndexes();

	allocateDirectory();
}

/**************************************************************************************************************/

template<typename T>
MyDeque< T >::MyDeque( int _elementsEmount, T _element )
	:MyDeque()
{
	for ( int i = 0; i < _elementsEmount; i++ )
		push_back( _element );
}

/**************************************************************************************************************/

template< typename T >
template< typename U >
MyDeque< T >::MyDeque( std::initializer_list< U > _list )
	:MyDeque()
{
	for ( const U & x : _list )
		this->push_back( x );
}

/**************************************************************************************************************/

template< typename T >
template< typename U >
MyDeque< T >::MyDeque( const MyDeque< U > & _deque )
	:MyDeque()
{
	copyElements( _deque );
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T >::MyDeque( const MyDeque< T > & _deque )
	:MyDeque()
{
	copyElements( _deque );
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T >::MyDeque( MyDeque< T > && _deque )
	:m_directorySize( _deque.m_directorySize ),
	m_backIndex( _deque.m_backIndex ),
	m_frontIndex( _deque.m_frontIndex ),
	m_backBlockIndex( _deque.m_backBlockIndex ),
	m_frontBlockIndex( _deque.m_frontBlockIndex ),
	m_backAllocatedBlockIndex( _deque.m_backAllocatedBlockIndex ),
	m_frontAllocatedBlockIndex( _deque.m_frontAllocatedBlockIndex )
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

	allocateDirectory();

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

	allocateDirectory();

	copyElements( _deque );

	return *this;
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T > & MyDeque< T >::operator = ( MyDeque< T > && _deque )
{
	if ( this == &_deque )
		return *this;

	std::swap( m_directorySize, _deque.m_directorySize );
	std::swap( m_frontIndex, _deque.m_frontIndex );
	std::swap( m_backIndex, _deque.m_backIndex );
	std::swap( m_frontBlockIndex, _deque.m_frontBlockIndex );
	std::swap( m_backBlockIndex, _deque.m_backBlockIndex );
	std::swap( m_frontAllocatedBlockIndex, _deque.m_frontAllocatedBlockIndex );
	std::swap( m_backAllocatedBlockIndex, _deque.m_backAllocatedBlockIndex );
	std::swap( m_data, _deque.m_data );
}

/**************************************************************************************************************/

template< typename T >
bool MyDeque< T >::operator == ( const MyDeque< T > & _deque ) const
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
bool MyDeque< T >::operator != ( const MyDeque< T > & _deque ) const
{
	return !( *this == _deque );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::push_back( const T & _element )
{
	nextIndex( m_backIndex, m_backBlockIndex, actions::INCREASE );

	allocateMemory();

	new ( m_data[m_backBlockIndex] + m_backIndex * sizeof( T ) ) T( _element );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::pop_back()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	deleteElement( m_data[m_backBlockIndex] + m_backIndex * sizeof( T ) );

	previousIndex( m_backIndex, m_backBlockIndex, actions::REDUCE );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::push_front( const T & _element )
{
	previousIndex( m_frontIndex, m_frontBlockIndex, actions::REDUCE );

	allocateMemory();

	new ( m_data[m_frontBlockIndex] + m_frontIndex * sizeof( T ) ) T( _element );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::pop_front()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	deleteElement( m_data[m_frontBlockIndex] + m_frontIndex * sizeof( T ) );

	nextIndex( m_frontIndex, m_frontBlockIndex, actions::INCREASE );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::clear()
{
	if ( empty() )
		return;

	resetData();

	resetIndexes();

	allocateDirectory();
}

/**************************************************************************************************************/

template< typename T >
bool MyDeque< T >::empty() const
{
	return size() == 0;
}

/**************************************************************************************************************/

template< typename T >
std::size_t MyDeque< T >::size() const
{
	if ( m_backBlockIndex < m_frontBlockIndex )
		return 0;

	if ( m_frontBlockIndex == m_backBlockIndex )
		return ( m_backIndex - m_frontIndex + 1 );

	size_t size = 0;
	size += m_blockSize - m_frontIndex;
	size += ( m_backBlockIndex - m_frontBlockIndex - 1 ) * m_blockSize;
	size += m_backIndex + 1;
	return size;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::front() const
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	T * result = getElement( m_data[m_frontBlockIndex] + m_frontIndex * sizeof( T ) );

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::back() const
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	T * result = getElement( m_data[backBlockIndex] + m_backIndex * sizeof( T ) );

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::front()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	T * result = getElement( m_data[m_frontBlockIndex] + m_frontIndex * sizeof( T ) );

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::back()
{
	if ( empty() )
		throw std::logic_error( Messages::EmptyDeque );

	T * result = getElement( m_data[m_backBlockIndex] + m_backIndex * sizeof( T ) );

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::operator [] ( int _index ) const
{
	auto toReturn = calculateIndex( _index );

	T * result = getElement( m_data[toReturn.first] + toReturn.second * sizeof( T ) );

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::operator [] ( int _index )
{
	auto toReturn = calculateIndex( _index );

	T * result = getElement( m_data[toReturn.first] + toReturn.second * sizeof( T ) );

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
	if ( !isValid( _index ) )
		throw std::logic_error( Messages::InvalidIndex );

	return ( *this )[_index];
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::grow()
{
	m_directorySize *= 2;
	unsigned char ** newDirectory = new unsigned char*[m_directorySize];

	int diffFront = m_frontAllocatedBlockIndex - m_frontBlockIndex;
	int diffBack = m_backAllocatedBlockIndex - m_backBlockIndex;
	int delta = m_directorySize / 2 - ( m_backAllocatedBlockIndex - m_frontAllocatedBlockIndex + 1 ) / 2;

	int counter = delta;
	for ( int i = m_frontAllocatedBlockIndex; i <= m_backAllocatedBlockIndex; i++ )
	{
		newDirectory[counter] = m_data[i];
		counter++;
	}

	m_frontAllocatedBlockIndex = delta;
	m_backAllocatedBlockIndex = counter - 1;

	m_frontBlockIndex = m_frontAllocatedBlockIndex - diffFront;
	m_backBlockIndex = m_backAllocatedBlockIndex - diffBack;

	delete[] m_data;
	m_data = newDirectory;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::shrink_to_fit()
{
	for ( int i = m_frontAllocatedBlockIndex; i < m_frontBlockIndex; i++ )
		delete[] m_data[i];
	for ( int i = m_backAllocatedBlockIndex; i > m_backBlockIndex; i-- )
		delete[] m_data[i];

	m_directorySize = m_backBlockIndex - m_frontBlockIndex + 1;
	int counter = m_frontBlockIndex;

	unsigned char ** shrinked_data = new unsigned char*[m_directorySize];
	for ( int i{ 0 }; i < m_directorySize; i++ )
	{
		shrinked_data[i] = m_data[counter];
		++counter;
	}

	m_backAllocatedBlockIndex = m_backBlockIndex = m_directorySize - 1;
	m_frontAllocatedBlockIndex = m_frontBlockIndex = 0;

	delete[] m_data;
	m_data = shrinked_data;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::nextIndex( int & _index, int & _blockIndex, int _action )
{
	assert( _index >= 0 );
	assert( _index <= m_blockSize );

	if ( _index == m_blockSize - 1 )
	{
		_blockIndex += _action;
		_index = 0;
		return;
	}
	++_index;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::previousIndex( int & _index, int & _blockIndex, int _action )
{
	assert( _index >= 0 );
	assert( _index <= m_blockSize );

	if ( _index == 0 )
	{
		_blockIndex += _action;
		_index = m_blockSize - 1;
		return;
	}
	--_index;
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
void MyDeque<T>::resetIndexes()
{
	m_directorySize = 2;

	m_backIndex = 9;
	m_frontIndex = 0;

	m_frontBlockIndex = m_directorySize / 2;
	m_backBlockIndex = m_directorySize / 2 - 1;

	m_frontAllocatedBlockIndex = m_frontBlockIndex;
	m_backAllocatedBlockIndex = m_backBlockIndex;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::deleteAllElements()
{
	if ( empty() )
		return;

	if ( std::is_class<T>::value )
	{
		if ( m_frontBlockIndex == m_backBlockIndex )
			for ( int i{ m_frontIndex }; i < m_backIndex; ++i )
				deleteElement( m_data[m_frontBlockIndex] + i * sizeof( T ) );

		else
		{
			for ( int i{ m_frontIndex }; i < m_blockSize; ++i )
				deleteElement( m_data[m_frontBlockIndex] + i * sizeof( T ) );

			for ( int i{ m_frontBlockIndex + 1 }; i < m_backBlockIndex; ++i )
				for ( int k{ 0 }; k < m_blockSize; ++k )
					deleteElement( m_data[i] + k * sizeof( T ) );

			for ( int i{ 0 }; i < m_backIndex; ++i )
				deleteElement( m_data[m_backBlockIndex] + i * sizeof( T ) );
		}
	}
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::freeMemoryBlocks()
{
	for ( int i = m_frontAllocatedBlockIndex; i <= m_backAllocatedBlockIndex; i++ )
		delete[] m_data[i];

	delete[] m_data;
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
typename MyDeque< T >::iterator MyDeque< T >::begin()
{
	iterator it( *this, 0 );
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::iterator MyDeque< T >::end()
{
	iterator it( *this, size() );
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::const_iterator MyDeque< T >::cbegin() const
{
	const_iterator it( *this, 0 );
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::const_iterator MyDeque< T >::cend() const
{
	const_iterator it( *this, size() );
	return it;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::erase( iterator _it )
{
	if ( !isValid( _it.getCurrentPosition() ) )
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
		!isValid( _itBegin.getCurrentPosition() ) ||
		!isValid( _itEnd.getCurrentPosition() ) )
		throw std::logic_error( Messages::InvalidIterator );

	int lastIndex = size() - 1;

	int copyFront = _itBegin.getCurrentPosition();
	int copyBack = lastIndex - _itEnd.getCurrentPosition();

	if ( copyBack > copyFront )
	{
		--_itBegin;

		for ( ; _itBegin.getCurrentPosition() >= 0; --_itBegin, --_itEnd )
			*_itEnd = std::move( *_itBegin );

		int difference = _itEnd.getCurrentPosition() - _itBegin.getCurrentPosition();
		for ( int i{ 0 }; i < difference; ++i )
			pop_front();
	}
	else
	{
		++_itEnd;

		for ( ; _itEnd.getCurrentPosition() <= lastIndex; ++_itBegin, ++_itEnd )
			*_itBegin = std::move( *_itEnd );

		int difference = _itEnd.getCurrentPosition() - _itBegin.getCurrentPosition();
		for ( int i{ 0 }; i < difference; ++i )
			pop_back();
	}
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::insert( iterator _it, const T & _element ) // fix this func
{
	if ( !isValid( _it.getCurrentPosition() ) )
		throw std::logic_error( Messages::InvalidIterator );

	int size = this->size();

	if ( _it.getCurrentPosition() > size / 2 )
	{
		nextIndex( m_backIndex, m_backBlockIndex, actions::INCREASE );
		allocateMemory();

		iterator itCurrent = end() - 1;
		iterator itNext = itCurrent - 1;

		for ( ; itCurrent != _it; --itNext, --itCurrent )
			*itCurrent = std::move( *itNext );
	}
	else
	{
		previousIndex( m_frontIndex, m_frontBlockIndex, actions::REDUCE );
		allocateMemory();

		iterator itCurrent = begin();
		iterator itNext = itCurrent + 1;

		for ( ; itCurrent != _it;  ++itNext, ++itCurrent )
			*itCurrent = std::move( *itNext );
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
std::pair<int, int> MyDeque< T >::calculateIndex( int _index ) const
{
	int blockIndex;
	int elementIndex;
	if ( _index < m_blockSize - m_frontIndex )
	{
		blockIndex = m_frontBlockIndex;
		elementIndex = m_frontIndex + _index;
	}
	else
	{
		_index -= m_blockSize - m_frontIndex;
		blockIndex = m_frontBlockIndex + _index / m_blockSize + 1;
		elementIndex = _index % m_blockSize;
	}
	return std::make_pair( blockIndex, elementIndex );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::allocateMemory()
{
	if ( m_frontBlockIndex == -1 || m_backBlockIndex == m_directorySize )
		grow();

	if ( m_frontBlockIndex < m_frontAllocatedBlockIndex )
		allocateBlock( m_frontBlockIndex, m_frontAllocatedBlockIndex );

	if ( m_backBlockIndex > m_backAllocatedBlockIndex )
		allocateBlock( m_backBlockIndex, m_backAllocatedBlockIndex );
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::allocateBlock( int & _blockIndex, int & _allocatedBlockIndex )
{
	m_data[_blockIndex] = new unsigned char[m_blockSize * sizeof( T )];

	_allocatedBlockIndex = _blockIndex;
}

/**************************************************************************************************************/

template< typename T >
inline T * MyDeque< T >::getElement( unsigned char * _pElement ) const
{
	return reinterpret_cast< T* >( _pElement );
}

/**************************************************************************************************************/

template<typename T>
inline void MyDeque< T >::deleteElement( unsigned char * _pElement )
{
	if ( std::is_class< T >::value )
		getElement( _pElement )->~T();
}

/**************************************************************************************************************/

template<typename T>
inline void MyDeque<T>::allocateDirectory()
{
	m_data = new unsigned char*[m_directorySize];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque >::Iterator( Deque & _deque, unsigned int _currentPositon )
	:m_deque( _deque ), m_currentPosition( _currentPositon )
{
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque >::Iterator( const Iterator<Deque> & _it )
	:m_deque( _it.m_deque ), m_currentPosition( _it.m_currentPosition )
{
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator == ( const Iterator< Deque > & _it ) const
{
	bool result = ( m_currentPosition == _it.m_currentPosition ) &&
		( ( const void * )&m_deque == ( const void * )&_it.m_deque );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator != ( const Iterator< Deque > & _it ) const
{
	return !( *this == _it );
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator >= ( const Iterator< Deque > & _it ) const
{
	return m_currentPosition >= _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator <= ( const Iterator< Deque > & _it ) const
{
	return m_currentPosition <= _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator > ( const Iterator< Deque > & _it ) const
{
	return m_currentPosition > _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
bool MyDeque< T >::Iterator< Deque >::operator < ( constIterator< Deque > & _it ) const
{
	return m_currentPosition < _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
template < class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator + ( int _step ) const
{
	//assert(m_currentPosition + _step < m_deque.size());

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition + _step );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator - ( int _step ) const
{
	//assert(m_currentPosition - _step >= 0);

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition - _step );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator + ( const Iterator< Deque > & _it ) const
{
	//assert( (m_currentPosition + _it.m_currentPosition) < m_deque.size());

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition + _it.m_currentPosition );

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator - ( const Iterator< Deque > & _it ) const
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
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator -= ( int _step )
{
	m_currentPosition -= _step;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator += ( int _step )
{
	m_currentPosition += _step;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator ++ ()
{
	//assert(m_currentPosition < m_deque.size());

	++m_currentPosition;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator ++ ( int )
{
	//assert(m_currentPosition < m_deque.size());

	MyDeque< T >::Iterator< Deque > result( m_deque, m_currentPosition );
	++m_currentPosition;

	return result;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > & MyDeque< T >::Iterator< Deque >::operator -- ()
{
	//assert(m_currentPosition > 0);

	--m_currentPosition;

	return *this;
}

/**************************************************************************************************************/

template< typename T >
template< class Deque >
MyDeque< T >::Iterator< Deque > MyDeque< T >::Iterator< Deque >::operator -- ( int )
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
int MyDeque< T >::Iterator< Deque >::getCurrentPosition() const
{
	return m_currentPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif