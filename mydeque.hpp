#ifndef MYDEQUE
#define MYDEQUE

/************************************************************************************/

#include "messages.hpp"

#include <type_traits>
#include <cassert>
#include <iostream>
#include <cstddef>
#include <iterator>

/************************************************************************************/

template< typename T >
class MyDeque
{
	/*----------------------------------------------------------------------------------*/

	const int m_blockSize = 10;

	int m_directorySize;

	int m_frontIndex, m_backIndex;

	int m_frontBlockIndex, m_backBlockIndex;

	int m_frontAllocatedBlockIndex, m_backAllocatedBlockIndex;

	unsigned char ** m_data;

	enum actions { REDUCE = -1, INCREASE = 1 };

	/*----------------------------------------------------------------------------------*/

	template< typename U >
	void copyElements(const MyDeque< U > & _deque);

	void grow();

	void resetData();

	void resetIndexes();

	void deleteAllElements();

	void freeMemoryBlocks();

	bool isValid(int _index) const { return (_index >= 0) && (_index < size()); }

	void nextIndex(int & _index, int & _blockIndex, int _action);

	void previousIndex(int & _index, int & _blockIndex, int _action);

	T * getElement(unsigned char * _pElement) const;

	void deleteElement(unsigned char * _pElement);

	void allocateDirectory();

	std::pair<int, int> calculateIndex(int _index) const; // first => blockIndex, second => elementIndex

	void allocateMemory();

	void allocateBlock(int & _blockIndex, int & _allocatedBlockIndex);

	/*----------------------------------------------------------------------------------*/

public:

	/*----------------------------------------------------------------------------------*/

	template< typename > friend class MyDeque;

	MyDeque();

	MyDeque(int _elementsEmount, T _element = T());

	template< typename U > MyDeque(std::initializer_list< U > _list);

	template< typename U > MyDeque(const MyDeque< U > & _deque);

	MyDeque(const MyDeque< T > & _deque);

	MyDeque(MyDeque< T > && _deque);

	~MyDeque();

	/*----------------------------------------------------------------------------------*/

	template< typename U > MyDeque< T > & operator = (const MyDeque< U > & _deque);

	MyDeque< T > & operator = (const MyDeque< T > & _deque);

	MyDeque< T > & operator = (MyDeque< T > && _deque);

	bool operator == (const MyDeque< T > & _deque) const;

	bool operator != (const MyDeque< T > & _deque) const;

	/*----------------------------------------------------------------------------------*/

	const T & operator [] (int _index) const;

	T & operator [] (int _index);

	const T & at(int _index) const;

	T & at(int _index);

	const T & front() const;

	T & front();

	const T & back() const;

	T & back();

	/*----------------------------------------------------------------------------------*/

	void push_back(const T & _element);

	void pop_back();

	void push_front(const T & _element);

	void pop_front();

	/*----------------------------------------------------------------------------------*/

	//void shrink_to_fit();

	std::size_t size() const;

	bool empty() const;

	void clear();

	/*----------------------------------------------------------------------------------*/

	class Iterator
	{
		/*----------------------------------------------------------------------------------*/

		size_t m_currentPosition;

		MyDeque< T > & m_deque;

		/*----------------------------------------------------------------------------------*/

	public:

		Iterator(MyDeque< T > & _deque, unsigned int _currentPositon);

		size_t getCurrentPosition() const;

		/*----------------------------------------------------------------------------------*/

		bool operator == (typename MyDeque< T >::Iterator _it) const;

		bool operator != (typename MyDeque< T >::Iterator _it) const;

		bool operator >= (typename MyDeque< T >::Iterator _it) const;

		bool operator <= (typename MyDeque< T >::Iterator _it) const;

		bool operator > (typename MyDeque< T >::Iterator _it) const;

		bool operator < (typename MyDeque< T >::Iterator _it) const;

		/*----------------------------------------------------------------------------------*/

		typename MyDeque< T >::Iterator operator + (int _step) const;

		typename MyDeque< T >::Iterator operator - (int _step) const;

		typename MyDeque< T >::Iterator & operator -= (int _step);

		typename MyDeque< T >::Iterator & operator += (int _step);

		/*----------------------------------------------------------------------------------*/

		typename MyDeque< T >::Iterator & operator + (typename MyDeque< T >::Iterator _it);

		typename MyDeque< T >::Iterator & operator - (typename MyDeque< T >::Iterator _it);

		typename MyDeque< T >::Iterator & operator = (typename MyDeque< T >::Iterator _it);

		typename MyDeque< T >::Iterator & operator -= (typename MyDeque< T >::Iterator _it);

		typename MyDeque< T >::Iterator & operator += (typename MyDeque< T >::Iterator _it);

		/*----------------------------------------------------------------------------------*/

		typename MyDeque< T >::Iterator & operator ++ ();

		typename MyDeque< T >::Iterator & operator ++ (int);

		typename MyDeque< T >::Iterator & operator -- ();

		typename MyDeque< T >::Iterator & operator -- (int);

		/*----------------------------------------------------------------------------------*/

		T operator *() const;

		T & operator *();
	};

	/*----------------------------------------------------------------------------------*/

	typedef const Iterator const_iterator;

	typedef Iterator iterator;

	void erase(Iterator _it);

	void erase(Iterator _itBegin, Iterator _itEnd);

	void insert(Iterator _it, const T & _element);

	template< class InputIt >
	void insert(InputIt _itBegin, InputIt _itEnd);

	Iterator begin();

	Iterator end();

	const Iterator cbegin() const;

	const Iterator cend() const;
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
MyDeque< T >::MyDeque(int _elementsEmount, T _element)
	:MyDeque()
{
	for ( int i = 0; i < _elementsEmount; i++ )
		push_back(_element);
}

/**************************************************************************************************************/

template< typename T >
	template< typename U >
MyDeque< T >::MyDeque(std::initializer_list< U > _list)
	:MyDeque()
{
	for ( const U & x : _list )
		this->push_back(x);
}

/**************************************************************************************************************/

template< typename T >
	template< typename U >
MyDeque< T >::MyDeque(const MyDeque< U > & _deque)
	:MyDeque()
{
	copyElements(_deque);
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T >::MyDeque(const MyDeque< T > & _deque)
	:MyDeque()
{
	copyElements(_deque);
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T >::MyDeque(MyDeque< T > && _deque)
	:m_directorySize(_deque.m_directorySize), m_frontIndex(_deque.m_frontIndex), m_backIndex(_deque.m_backIndex),
	m_frontBlockIndex(_deque.m_frontBlockIndex), m_backBlockIndex(_deque.m_backBlockIndex),
	m_backAllocatedBlockIndex(_deque.m_backAllocatedBlockIndex),
	m_frontAllocatedBlockIndex(_deque.m_frontAllocatedBlockIndex)
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
MyDeque< T > & MyDeque< T >::operator = (const MyDeque< U > & _deque)
{
	if ( (const void *) (this) == (const void *) (&_deque) )
		return *this;

	clear();

	allocateDirectory();

	copyElements(_deque);

	return *this;
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T > & MyDeque< T >::operator = (const MyDeque< T > & _deque)
{
	if ( this == &_deque )
		return *this;

	clear();

	allocateDirectory();

	copyElements(_deque);

	return *this;
}

/**************************************************************************************************************/

template< typename T >
MyDeque< T > & MyDeque< T >::operator = (MyDeque< T > && _deque)
{
	if ( this == &_deque )
		return *this;

	std::swap(m_directorySize, _deque.m_directorySize);
	std::swap(m_frontIndex, _deque.m_frontIndex);
	std::swap(m_backIndex, _deque.m_backIndex);
	std::swap(m_frontBlockIndex, _deque.m_frontBlockIndex);
	std::swap(m_backBlockIndex, _deque.m_backBlockIndex);
	std::swap(m_frontAllocatedBlockIndex, _deque.m_frontAllocatedBlockIndex);
	std::swap(m_backAllocatedBlockIndex, _deque.m_backAllocatedBlockIndex);
	std::swap(m_data, _deque.m_data);
}

/**************************************************************************************************************/

template< typename T >
bool MyDeque< T >::operator == (const MyDeque< T > & _deque) const
{
	int size = this->size();

	if ( size != _deque.size() )
		return false;

	for ( int i = 0; i < size; i++ )
		if ( (*this)[i] != _deque[i] )
			return false;

	return true;
}

/**************************************************************************************************************/

template< typename T >
bool MyDeque< T >::operator != (const MyDeque< T > & _deque) const
{
	return !(*this == _deque);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::push_back(const T & _element)
{
	nextIndex(m_backIndex, m_backBlockIndex, actions::INCREASE);

	allocateMemory();

	new (m_data[m_backBlockIndex] + m_backIndex * sizeof(T)) T(_element);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::pop_back()
{
	if ( empty() )
		throw std::logic_error(Messages::EmptyDeque);

	deleteElement(m_data[m_backBlockIndex] + m_backIndex * sizeof(T));

	previousIndex(m_backIndex, m_backBlockIndex, actions::REDUCE);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::push_front(const T & _element)
{
	previousIndex(m_frontIndex, m_frontBlockIndex, actions::REDUCE);

	allocateMemory();

	new (m_data[m_frontBlockIndex] + m_frontIndex * sizeof(T)) T(_element);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::pop_front()
{
	if ( empty() )
		throw std::logic_error(Messages::EmptyDeque);

	deleteElement(m_data[m_frontBlockIndex] + m_frontIndex * sizeof(T));

	nextIndex(m_frontIndex, m_backBlockIndex, actions::REDUCE);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::clear()
{
	if ( empty() )
		return;

	resetData();

	resetIndexes();
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
		return (m_backIndex - m_frontIndex + 1);

	size_t size = 0;
	size += m_blockSize - m_frontIndex;
	size += (m_backBlockIndex - m_frontBlockIndex - 1) * m_blockSize;
	size += m_backIndex + 1;
	return size;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::front() const
{
	if ( empty() )
		throw std::logic_error(Messages::EmptyDeque);

	T * result = getElement(m_data[m_frontBlockIndex] + m_frontIndex * sizeof(T));

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::back() const
{
	if ( empty() )
		throw std::logic_error(Messages::EmptyDeque);

	T * result = getElement(m_data[backBlockIndex] + m_backIndex * sizeof(T));

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::front()
{
	if ( empty() )
		throw std::logic_error(Messages::EmptyDeque);

	T * result = getElement(m_data[m_frontBlockIndex] + m_frontIndex * sizeof(T));

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::back()
{
	if ( empty() )
		throw std::logic_error(Messages::EmptyDeque);

	T * result = getElement(m_data[m_backBlockIndex] + m_backIndex * sizeof(T));

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::operator [] (int _index) const
{
	auto toReturn = calculateIndex(_index);

	T * result = getElement(m_data[toReturn.first] + toReturn.second * sizeof(T));

	return *result;
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::operator [] (int _index)
{
	auto toReturn = calculateIndex(_index);

	T * result = getElement(m_data[toReturn.first] + toReturn.second * sizeof(T));

	return *result;
}

/**************************************************************************************************************/

template< typename T >
const T & MyDeque< T >::at(int _index) const
{
	if ( !isValid(_index) )
		throw std::logic_error(Messages::InvalidIndex);

	return (*this)[_index];
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::at(int _index)
{
	if ( !isValid(_index) )
		throw std::logic_error(Messages::InvalidIndex);

	return (*this)[_index];
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::grow()
{
	m_directorySize *= 2;
	unsigned char ** newDirectory = new unsigned char*[m_directorySize];

	int diffFront = m_frontAllocatedBlockIndex - m_frontBlockIndex;
	int diffBack = m_backAllocatedBlockIndex - m_backBlockIndex;
	int delta = m_directorySize / 2 - (m_backAllocatedBlockIndex - m_frontAllocatedBlockIndex + 1) / 2;

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

//template< typename T >
//void MyDeque< T >::shrink_to_fit()
//{
//	for (int i = m_frontAllocatedBlockIndex; i < m_frontBlockIndex; i++)
//		delete[] m_data[i];
//	for (int i = m_backAllocatedBlockIndex; i > m_backBlockIndex; i--)
//		delete[] m_data[i];
//
//	m_directorySize = m_backBlockIndex - m_frontBlockIndex + 1;
//	int counter = m_frontBlockIndex;
//
//	unsigned char ** shrinked_data = new unsigned char*[m_directorySize];
//	for (int i = 0; i < m_directorySize; i++)
//	{
//		shrinkedm_data[i] = m_data[counter];
//		counter++;
//	}
//
//	m_backAllocatedBlockIndex = m_backBlockIndex = m_directorySize - 1;
//	m_frontAllocatedBlockIndex = m_frontBlockIndex = 0;
//
//	delete[] m_data;
//	m_data = shrinked_data;
//}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::nextIndex(int & _index, int & _blockIndex, int _action)
{
	assert(_index >= 0);
	assert(_index <= m_blockSize);

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
void MyDeque< T >::previousIndex(int & _index, int & _blockIndex, int _action)
{
	assert(_index >= 0);
	assert(_index <= m_blockSize);

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
				getElement(m_data[m_frontBlockIndex] + i * sizeof(T))->~T();

		else
		{
			for ( int i{ m_frontIndex }; i < m_blockSize; ++i )
				getElement(m_data[m_frontBlockIndex] + i * sizeof(T))->~T();

			for ( int i{ m_frontBlockIndex + 1 }; i < m_backBlockIndex; ++i )
				for ( int k{ 0 }; k < m_blockSize; ++k )
					getElement(m_data[i] + k * sizeof(T))->~T();

			for ( int i{ 0 }; i < m_backIndex; ++i )
				getElement(m_data[m_backBlockIndex] + i * sizeof(T))->~T();
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

	m_data = nullptr; // for cases, when client does nothing after calling clear() func, for correct deleting
}

/**************************************************************************************************************/

template< typename T >
	template< typename U >
void MyDeque< T >::copyElements(const MyDeque< U > & _deque)
{
	int size = _deque.size();
	for ( int i = 0; i < size; i++ )
		push_back(_deque[i]);
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator MyDeque< T >::begin()
{
	Iterator it(*this, 0);
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::iterator MyDeque< T >::end()
{
	iterator it(*this, size() - 1);
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename const MyDeque< T >::Iterator MyDeque< T >::cbegin() const
{
	const Iterator it(*this, 0);
	return it;
}

/**************************************************************************************************************/

template< typename T >
typename const MyDeque< T >::Iterator MyDeque< T >::cend() const
{
	const Iterator it(*this, size() - 1);
	return it;
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::erase(typename MyDeque< T >::Iterator _it)
{
	if ( !isValid(_it.getCurrentPosition()) )
		throw std::logic_error(Messages::InvalidIterator);

	// TODO: choose the right side

	int size = this->size() - 2; // why?
	for ( int i = _it.getCurrentPosition(); i < size; i++ )
		(*this)[i] = (*this)[i + 1];

	previousIndex(m_backIndex, m_backBlockIndex, REDUCE);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::erase(typename MyDeque< T >::Iterator _itBegin, typename MyDeque< T >::Iterator _itEnd)
{
	if ( _itEnd.getCurrentPosition() < _itBegin.getCurrentPosition() ||
		!isValid(_itBegin.getCurrentPosition()) ||
		!isValid(_itEnd.getCurrentPosition()) )
		throw std::logic_error(Messages::InvalidIterator);

	int lastIndex = this->size() - 1; // -1 becouse it is index
	int difference = _itEnd.getCurrentPosition() - _itBegin.getCurrentPosition() + 1;

	for ( int i = _itBegin.getCurrentPosition(); i < _itBegin.getCurrentPosition() + difference; i++ )
	{
		if ( i + difference <= lastIndex )
			(*this)[i] = (*this)[i + difference];
		else
			break;
	}

	for ( int i = 0; i < difference; i++ )
		previousIndex(m_backIndex, m_backBlockIndex, actions::REDUCE);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::insert(typename MyDeque< T >::Iterator _it, const T & _element)
{
	if ( !isValid(_it.getCurrentPosition()) )
		throw std::logic_error(Messages::InvalidIterator);

	nextIndex(m_backIndex, m_backBlockIndex, actions::INCREASE);

	allocateMemory();

	int index = _it.getCurrentPosition();
	for ( int i = size() - 1; i > index; i-- )
		(*this)[i] = (*this)[i - 1];

	(*this)[index] = _element;
}

/**************************************************************************************************************/

template< typename T >
template< class InputIt >
void MyDeque< T >::insert(InputIt _itBegin, InputIt _itEnd)
{
	// TODO: push only one element if equal

	do {
		push_back(*_itBegin);
		++_itBegin;
	} while ( _itBegin != _itEnd );
}

/**************************************************************************************************************/

template< typename T >
std::pair<int, int> MyDeque< T >::calculateIndex(int _index) const
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
	return std::make_pair(blockIndex, elementIndex);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::allocateMemory()
{
	if ( m_frontBlockIndex == -1 || m_backBlockIndex == m_directorySize )
		grow();

	if ( m_frontBlockIndex < m_frontAllocatedBlockIndex )
		allocateBlock(m_frontBlockIndex, m_frontAllocatedBlockIndex);

	if ( m_backBlockIndex > m_backAllocatedBlockIndex )
		allocateBlock(m_backBlockIndex, m_backAllocatedBlockIndex);
}

/**************************************************************************************************************/

template< typename T >
void MyDeque< T >::allocateBlock(int & _blockIndex, int & _allocatedBlockIndex)
{
	m_data[_blockIndex] = new unsigned char[m_blockSize * sizeof(T)];

	_allocatedBlockIndex = _blockIndex;
}

/**************************************************************************************************************/

template< typename T >
inline T * MyDeque< T >::getElement(unsigned char * _pElement) const
{
	return reinterpret_cast<T*>(_pElement);
}

/**************************************************************************************************************/

template<typename T>
inline void MyDeque<T>::deleteElement(unsigned char * _pElement)
{
	if ( std::is_class<T>::value )
		getElement(_pElement)->~T();
}

/**************************************************************************************************************/

template<typename T>
inline void MyDeque<T>::allocateDirectory()
{
	m_data = new unsigned char*[m_directorySize];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename T >
MyDeque< T >::Iterator::Iterator(MyDeque< T > & _deque, unsigned int _currentPositon)
	:m_deque(_deque), m_currentPosition(_currentPositon)
{}

/**************************************************************************************************************/

template< typename T >
bool typename MyDeque< T >::Iterator::operator == (typename MyDeque< T >::Iterator _it) const
{
	return m_currentPosition == _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
bool typename MyDeque< T >::Iterator::operator != (typename MyDeque< T >::Iterator _it) const
{
	return m_currentPosition != _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
bool typename MyDeque< T >::Iterator::operator >= (typename MyDeque< T >::Iterator _it) const
{
	return m_currentPosition >= _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
bool typename MyDeque< T >::Iterator::operator <= (typename MyDeque< T >::Iterator _it) const
{
	return m_currentPosition <= _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
bool typename MyDeque< T >::Iterator::operator > (typename MyDeque< T >::Iterator _it) const
{
	return m_currentPosition > _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
bool typename MyDeque< T >::Iterator::operator < (typename MyDeque< T >::Iterator _it) const
{
	return m_currentPosition < _it.m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator MyDeque< T >::Iterator::operator + (int _step) const
{
	assert(m_currentPosition + _step < m_deque.size());
	MyDeque< T >::Iterator result = *this;
	result.m_currentPosition += _step;
	return result;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator MyDeque< T >::Iterator::operator - (int _step) const
{
	assert(m_currentPosition - _step >= 0);
	MyDeque< T >::Iterator result = *this;
	result.m_currentPosition -= _step;
	return result;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator + (typename MyDeque< T >::Iterator _it)
{
	assert(m_currentPosition + _it.currentPosition < m_deque.size());
	MyDeque< T >::Iterator result = *this;
	result.m_currentPosition += _it.m_currentPosition;
	return result;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator - (typename MyDeque< T >::Iterator _it)
{
	assert(m_currentPosition - _it.m_currentPosition >= 0)
		MyDeque< T >::Iterator result = *this;
	result.m_currentPosition -= _it.m_currentPosition;
	return result;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator = (typename MyDeque< T >::Iterator _it)
{
	m_currentPosition = _it.m_currentPosition;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator -= (typename MyDeque< T >::Iterator _it)
{
	assert(m_currentPosition - _it.m_currentPosition >= 0);
	m_currentPosition -= _it.m_currentPosition;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator += (typename MyDeque< T >::Iterator _it)
{
	m_currentPosition += _it.m_currentPosition;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator -= (int _step)
{
	m_currentPosition -= _step;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator += (int _step)
{
	m_currentPosition += _step;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator ++ ()
{
	assert(m_currentPosition < m_deque.size());
	++m_currentPosition;
	return *this;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator ++ (int)
{
	assert(m_currentPosition < m_deque.size());
	MyDeque< T >::Iterator result = *this;
	++m_currentPosition;
	return result;
}

/**************************************************************************************************************/

template< typename T >
typename typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator -- ()
{
	assert(m_currentPosition > 0);
	return --m_currentPosition;
}

/**************************************************************************************************************/

template< typename T >
typename MyDeque< T >::Iterator & MyDeque< T >::Iterator::operator -- (int)
{
	assert(m_currentPosition > 0);
	MyDeque< T >::Iterator result = *this;
	--m_currentPosition;
	return result;
}

/**************************************************************************************************************/

template< typename T >
T MyDeque< T >::Iterator::operator *() const
{
	return m_deque[m_currentPosition];
}

/**************************************************************************************************************/

template< typename T >
T & MyDeque< T >::Iterator::operator *()
{
	return m_deque[m_currentPosition];
}

/**************************************************************************************************************/

template< typename T >
size_t MyDeque< T >::Iterator::getCurrentPosition() const
{
	return m_currentPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif