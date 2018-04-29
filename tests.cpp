#include "mydeque.hpp"
#include "testlib.hpp"
#include <deque>
#include <stack>
#include <vector>

DECLARE_OOP_TEST( default_constructor )
{
	MyDeque<int> testInt;
	assert( testInt.size() == 0 );
	assert( testInt.empty() );

	MyDeque<double> testDouble;
	assert( testDouble.size() == 0 );
	assert( testDouble.empty() );

	MyDeque<std::string> testString;
	assert( testString.size() == 0 );
	assert( testString.empty() );

	MyDeque< std::vector<int> > testVector;
	assert( testVector.size() == 0 );
	assert( testVector.empty() );
}

DECLARE_OOP_TEST( constructor_with_amount_of_elements_and_their_value )
{
	MyDeque<int> testInt( 10, 0 );
	assert( testInt.size() == 10 );
	for ( int i = 0; i < 10; i++ )
		assert( testInt[i] == 0 );


	MyDeque<int> testInt2( 20, 3 );
	assert( testInt2.size() == 20 );
	for ( int i = 0; i < 20; i++ )
		assert( testInt2[i] == 3 );


	std::string exampleString = "hello";
	MyDeque<std::string> testString( 20, exampleString );
	assert( testString.size() == 20 );
	for ( int i = 0; i < 20; i++ )
		assert( testString[i] == exampleString );


	std::vector<int> exampleVector{ 1, 2, 3 };
	MyDeque< std::vector<int> > testVector( 30, exampleVector );
	assert( testVector.size() == 30 );
	for ( int i = 0; i < 20; i++ )
		assert( testVector[i] == exampleVector );
}

DECLARE_OOP_TEST( initializer_list_constructor )
{
	MyDeque<int> testInt{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	assert( testInt.size() == 10 );
	for ( int i = 0; i < 10; i++ )
		assert( testInt[i] == i + 1 );

	MyDeque<std::string> testString{ "hello", "guys" };
	assert( testString[0] == "hello" );
	assert( testString[1] == "guys" );

	std::vector<int> forInst1{ 1, 2, 3 };
	std::vector<int> forInst2{ 4, 5, 6 };
	MyDeque< std::vector<int> > testVector{ forInst1, forInst2 };
	assert( testVector[0] == forInst1 );
	assert( testVector[1] == forInst2 );
}

DECLARE_OOP_TEST( copy_constructor_with_same_type )
{
	MyDeque<int> testInt1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	MyDeque<int> testInt2( testInt1 );
	assert( testInt1.size() == testInt2.size() );
	for ( int i = 0; i < 10; i++ )
		assert( testInt2[i] == testInt1[i] );

	MyDeque<std::string> testString1{ "some", "string" };
	MyDeque<std::string> testString2( testString1 );
	assert( testString2[0] == "some" );
	assert( testString2[1] == "string" );
}

DECLARE_OOP_TEST( copy_constructor_with_different_type )
{
	MyDeque<double> testDouble{ 9.9, 8.8, 7.7, 6.6, 5.5, 4.4, 3.3, 2.2, 1.1 };
	MyDeque<int> testInt( testDouble );

	for ( int i = 0; i < 9; i++ )
		assert( testInt[i] == ( int ) ( testDouble[i] ) );

	assert( testDouble.size() == testInt.size() );
}

DECLARE_OOP_TEST( move_constructor )
{
	int elements[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

	MyDeque<int> test1;
	for ( auto number : elements )
		test1.push_back( number );

	MyDeque<int> test2 = std::move( test1 );

	for ( int i{ 0 }; i < 8; i++ )
		assert( test2[i] = elements[i] );

	//------------------------------------------------------

	std::vector< std::vector<int> > forCheck{
		{ 1, 1, 1, 1 }
	,	{ 2, 2, 2, 2, 2 }
	,	{ 3, 3, 3, 3, 3 }
	};

	MyDeque< std::vector<int> > testVector1;
	for ( auto & v : forCheck )
		testVector1.push_back( v );

	MyDeque< std::vector<int> > testVectors2 = std::move( testVector1 );
	for ( int i{ 0 }; i < 3; ++i )
		assert( testVectors2[i] == forCheck[i] );

}

DECLARE_OOP_TEST( compare_operators )
{
	MyDeque<int> test1{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	MyDeque<int> test2{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	MyDeque<int> test3{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	MyDeque<int> test4{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	assert( !( test1 == test2 ) );
	assert( test1 != test2 );
	assert( test1 == test3 );
	assert( test1 != test4 );


	MyDeque<std::string> test5{ "some", "strings" };
	MyDeque<std::string> test6{ "new", "strings" };
	assert( !( test5 == test6 ) );
	assert( test5 != test6 );
}

DECLARE_OOP_TEST( appropriation_operator_with_different_types )
{
	MyDeque<int> test1{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	MyDeque<double> test2{ 9.9, 8.8, 7.7, 6.6, 5.5, 4.4, 3.3, 2.2, 1.1 };
	test1 = test2;
	for ( int i = 0; i < 9; i++ )
		assert( test1[i] == ( int ) test2[i] );
}

DECLARE_OOP_TEST( appropriation_operator_with_same_types_and_check_for_creating_correct_copy )
{
	MyDeque<int> testInt1{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	{
		MyDeque<int> testInt2{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		testInt1 = testInt2;
	}

	for ( int i = 0; i < 9; i++ )
		assert( testInt1[i] == i + 1 );

	//--------------------------------------------------

	std::string data[] = { "some", "strings", "for", "test" };

	MyDeque< std::string > testStr1{ "not", "used" };
	{
		MyDeque< std::string > testStr2;
		for ( auto & it : data )
			testStr2.push_back( it );

		testStr1 = testStr2;
	}

	for ( int i{ 0 }; i < 4; i++ )
		assert( testStr1[i] == data[i] );
}

DECLARE_OOP_TEST( clear_and_empty )
{
	MyDeque<int> test1( 5 );
	assert( test1.size() == 5 );
	assert( !test1.empty() );
	test1.clear();
	assert( test1.empty() );

	MyDeque<int> test2( 15 );
	assert( test2.size() == 15 );
	assert( !test2.empty() );
	test2.clear();
	assert( test2.empty() );

	MyDeque<int> test3;
	test3.push_back( 1 );
	assert( test3.size() == 1 );
	test3.pop_back();
	assert( test3.empty() );

	MyDeque<std::string> test4;
	test4.push_back( "some string" );
	assert( test4.size() == 1 );
	test4.pop_back();
	assert( test4.empty() );
}

DECLARE_OOP_TEST( size )
{
	MyDeque<int> test( 8 );
	assert( test.size() == 8 );
	test.push_back( 1 );
	assert( test.size() == 9 );
	test.push_back( 1 );
	assert( test.size() == 10 );
	test.push_front( 1 );
	assert( test.size() == 11 );
	test.push_front( 1 );
	assert( test.size() == 12 );
	test.pop_back();
	test.pop_front();
	assert( test.size() == 10 );
}

DECLARE_OOP_TEST( const_at )
{
	MyDeque<int> test{ 0, 1, 2, 3, 4, 5 };

	for ( int i = 0; i <= 5; i++ )
		assert( test.at( i ) == i );
	try
	{
		test.at( 6 );
		assert( !"Exception must have been thrown" );
	}
	catch ( std::exception & e )
	{
		assert( !strcmp( e.what(), Messages::InvalidIndex ) );
	}

	MyDeque<std::string> test2{ "some", "strings", "for", "testing" };
	assert( test2.at( 0 ) == "some" );
	assert( test2.at( 1 ) == "strings" );
	assert( test2.at( 2 ) == "for" );
	assert( test2.at( 3 ) == "testing" );
	try
	{
		test2.at( 4 );
		assert( !"Exception must have been thrown" );
	}
	catch ( std::exception & e )
	{
		assert( !strcmp( e.what(), Messages::InvalidIndex ) );
	}
}

DECLARE_OOP_TEST( at_for_writing )
{
	MyDeque<int> test{ 0, 1, 2, 3, 4, 5 };

	for ( int i = 0; i <= 5; i++ )
		test.at( i ) = i + 10;

	for ( int i = 0; i <= 5; i++ )
		assert( test.at( i ) == i + 10 );
	try
	{
		test.at( 6 );
		assert( !"Exception must have been thrown" );
	}
	catch ( std::exception & e )
	{
		assert( !strcmp( e.what(), Messages::InvalidIndex ) );
	}

	MyDeque<std::string> test2( 4, "something" );
	for ( int i = 0; i < 4; i++ )
		test2.at( i ) = "new str";

	for ( int i = 0; i < 4; i++ )
		assert( test2.at( i ) == "new str" );
	try
	{
		test2.at( 5 );
		assert( !"Exception must have been thrown" );
	}
	catch ( std::exception & e )
	{
		assert( !strcmp( e.what(), Messages::InvalidIndex ) );
	}
}

DECLARE_OOP_TEST( index_operator )
{
	MyDeque<int> testInt{ 0, 1, 2, 3, 4, 5 };
	for ( int i = 0; i < 6; i++ )
		assert( testInt[i] == i );

	testInt.push_front( 1 );
	assert( testInt[0] == 1 );
	testInt.push_back( 1 );
	assert( testInt[testInt.size() - 1] == 1 );

	//------------------------------------------------------------------

	MyDeque<std::string> testStr{ "some", "strings", "for", "testing" };
	assert( testStr[0] == "some" );
	assert( testStr[1] == "strings" );
	assert( testStr[2] == "for" );
	assert( testStr[3] == "testing" );
}

DECLARE_OOP_TEST( writing_by_index_operator )
{
	MyDeque<int> test( 10, 5 );
	for ( int i = 0; i < 10; i++ )
		test[i] = 15;
	for ( int i = 0; i < 10; i++ )
		assert( test[i] == 15 );

	MyDeque<std::string> test2( 4, "something" );
	for ( int i = 0; i < 4; i++ )
		test2[i] = "new str";
	for ( int i = 0; i < 4; i++ )
		assert( test2[i] == "new str" );
}

DECLARE_OOP_TEST( front_and_back_const )
{
	MyDeque<int> test( 1 );
	assert( test.front() == test.back() );
	test.push_back( 1 );
	assert( test.back() == 1 );
	test.push_front( 9 );
	assert( test.front() == 9 );

	MyDeque<std::string> test2{ "some", "strings", "for", "testing" };
	assert( test2.front() == "some" );
	assert( test2.back() == "testing" );
}

DECLARE_OOP_TEST( front_and_back_with_writing )
{
	MyDeque<int> test( 3, 0 );
	test.front() = 1;
	assert( test[0] == 1 );
	test.back() = 2;
	assert( test[test.size() - 1] == 2 );

	MyDeque<std::string> test2{ "some", "strings", "for", "testing" };
	test2.front() = "new";
	test2.back() = "str";
	assert( test2.front() == "new" );
	assert( test2.back() == "str" );
}

DECLARE_OOP_TEST( pushBack_and_popBack )
{
	MyDeque< std::vector<int> > testVec;

	testVec.push_back( {} );
	assert( testVec.size() == 1 );

	testVec.pop_back();
	assert( testVec.size() == 0 );

	for ( int i = 1; i <= 100; i++ )
		testVec.push_back( {} );
	assert( testVec.size() == 100 );

	for ( int i = 0; i < 10; i++ )
		testVec.pop_back();
	assert( testVec.size() == 90 );

	for ( int i = 0; i < 10; i++ )
		testVec.pop_back();
	assert( testVec.size() == 80 );

	for ( int i = 0; i < 20; i++ )
		testVec.push_back( {} );
	assert( testVec.size() == 100 );

	//-------------------------------------

	std::vector< int > data{ 1, 2, 3, 4, 5, 6, 7, 8 };
	MyDeque< int > test;
	for ( auto it : data )
		test.push_back( it );

	data.push_back( 9 );
	test.push_back( 9 );

	int size = test.size();
	for ( int i{ 0 }; i < size; ++i )
		assert( test[i] == data[i] );

	for ( int i{ 0 }; i < 3; ++i )
	{
		data.pop_back();
		test.pop_back();
	}

	size = test.size();
	for ( int i{ 0 }; i < size; ++i )
		assert( test[i] == data[i] );
}

DECLARE_OOP_TEST( pushFront_and_popFront )
{
	MyDeque<int> test;

	test.push_front( 1 );
	assert( test.size() == 1 );
	test.pop_front();
	assert( test.size() == 0 );

	for ( int i = 1; i <= 100; i++ )
		test.push_front( i );
	assert( test.size() == 100 );

	for ( int i = 0; i < 10; i++ )
		test.pop_front();
	assert( test.size() == 90 );

	for ( int i = 0; i < 10; i++ )
		test.pop_front();
	assert( test.size() == 80 );

	for ( int i = 0; i < 20; i++ )
		test.push_front( i );
	assert( test.size() == 100 );

	//-------------------------------

	MyDeque< int > testInt{ 1, 2, 3, 4, 5, 6, 7, 8 };

	testInt.push_front( 0 );
	assert( testInt[0] == 0 );

	int size = testInt.size();
	for ( int i{ 0 }; i < size; ++i )
		assert( testInt[i] == i );

	for ( int i{ 0 }; i < 3; ++i )
		testInt.pop_front();

	size = testInt.size();
	for ( int i{ 0 }; i < size; ++i )
		assert( testInt[i] == i + 3 );
}

DECLARE_OOP_TEST( pushBack_and_popFront )
{
	MyDeque<int> test;

	test.push_back( 10 );
	test.pop_front();
	assert( test.empty() );

	for ( int i = 0; i < 10; i++ )
		test.push_back( 10 );

	for ( int i = 0; i < 10; i++ )
		test.pop_front();

	assert( test.empty() );
}

DECLARE_OOP_TEST( pushFront_and_popBack )
{
	MyDeque<int> test;

	test.push_front( 10 );
	test.pop_back();
	assert( test.empty() );

	for ( int i = 0; i < 10; i++ )
		test.push_front( 10 );

	for ( int i = 0; i < 10; i++ )
		test.pop_back();

	assert( test.empty() );
}

DECLARE_OOP_TEST( iterator_correct_values )
{
	MyDeque<int> test{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	auto it = test.begin();
	for ( int i = 0; i < 9; i++ )
	{
		assert( *it == test[i] );
		++it;
	}

	auto itC = test.cbegin();
	for ( int i = 0; i < 9; i++ )
	{
		assert( *itC == test[i] );
		++itC;
	}
}

DECLARE_OOP_TEST( iterator_operators )
{
	MyDeque<int> test{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	auto it1 = test.begin();
	auto it2 = test.begin();

	assert( it1 == it2 );
	assert( !( it1 != it2 ) );

	auto it3 = it1++;
	assert( it3 == it2 );

	++it2;
	assert( it1 == it2 );

	it1 = it1 + 1;
	it2++;
	assert( it1 == it2 );

	++it1;
	assert( it1 > it2 );
	assert( !( it1 < it2 ) );

	it1 -= 2;
	it2 -= 1;
	assert( it1 == it2 );

	it3 = it2--;
	assert( it3 == it1 );
	it1 = it1 - 1;
	assert( it2 == it1 );
}

DECLARE_OOP_TEST( const_iterator_operators )
{
	MyDeque<int> test{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	auto it1 = test.cbegin();
	auto it2 = test.cbegin();

	assert( it1 == it2 );
	assert( !( it1 != it2 ) );

	auto it3 = it1++;
	assert( it3 == it2 );

	++it2;
	assert( it1 == it2 );

	it1 = it1 + 1;
	it2++;
	assert( it1 == it2 );

	++it1;
	assert( it1 > it2 );
	assert( !( it1 < it2 ) );

	it1 -= 2;
	it2 -= 1;
	assert( it1 == it2 );

	it3 = it2--;
	assert( it3 == it1 );
	it1 = it1 - 1;
	assert( it2 == it1 );
}

DECLARE_OOP_TEST( writing_with_iterator )
{
	MyDeque<int> test{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	auto it = test.begin();
	for ( int i = 0; i < 9; i++ )
	{
		*it = i;
		++it;
	}
	it = test.begin();
	for ( int i = 0; i < 9; i++ )
	{
		assert( *it == i );
		++it;
	}
}

DECLARE_OOP_TEST( insert_one_element_first_half )
{
	MyDeque<int> test{ 1, 2, 4, 5, 6, 7, 8, 9 };
	auto it = test.begin();
	it += 2;
	test.insert( it, 3 );
	int size = test.size();
	for ( int i = 0; i < size; i++ )
		assert( test[i] == i + 1 );
}

DECLARE_OOP_TEST( insert_one_element_second_half )
{
	MyDeque<int> test{ 1, 2, 3, 4, 5, 6, 7, 9 };
	auto it = test.end();
	--it;
	test.insert( it, 8 );
	int size = test.size();
	for ( int i = 0; i < size; i++ )
		assert( test[i] == i + 1 );
}

DECLARE_OOP_TEST( insert_sequence_first_half )
{
	MyDeque<int> test{ 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
	auto itDeque = test.begin();
	++itDeque;

	std::vector<int> v{ 2, 3, 4 };
	auto itVecBegin = v.begin();
	auto itVecEnd = v.end();
	test.insert( itVecBegin, itVecEnd, itDeque );

	int size = test.size();
	for ( int i = 0; i < size; i++ )
		assert( test[i] == i + 1 );
}

DECLARE_OOP_TEST( insert_sequence_second_half )
{
	MyDeque<int> test{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 22 };
	auto itDeque = test.end();
	--itDeque;

	std::vector<int> v( { 19, 20, 21 } );
	auto itVecBegin = v.begin();
	auto itVecEnd = v.end();
	test.insert( itVecBegin, itVecEnd, itDeque );

	int size = test.size();
	for ( int i = 0; i < size; i++ )
		assert( test[i] == i + 1 );
}

DECLARE_OOP_TEST( erase_one_element_first_half )
{
	MyDeque<int> test{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	test.erase( test.begin() );
	assert( test[0] == 2 );

	test.erase( test.begin() + 1 );
	assert( test[1] == 4 );
}

DECLARE_OOP_TEST( erase_one_element_second_half )
{
	MyDeque<int> test{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int lastIndex = test.size() - 1;

	test.erase( test.end() - 1 );
	lastIndex -= 1;
	assert( test[lastIndex] == 8 );

	test.erase( test.end() - 3 );
	lastIndex -= 1;
	assert( test[lastIndex - 1] == 7 );
}

DECLARE_OOP_TEST( erase_sequence_first_half )
{
	MyDeque<int> test{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	assert( test.size() == 9 );

	auto it1 = test.begin();
	auto it2 = test.begin();
	it2 += 2;
	test.erase( it1, it2 );

	int size = test.size();
	assert( size == 6 );

	for ( int i{ 0 }; i < size; i++ )
		assert( test[i] == i + 4 );
}

DECLARE_OOP_TEST( erase_sequence_second_half )
{
	MyDeque<int> test{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	assert( test.size() == 9 );

	auto it1 = test.end();
	auto it2 = test.end();
	it1 -= 3;
	it2 -= 1;
	test.erase( it1, it2 );

	int size = test.size();
	assert( size == 6 );

	for ( int i{ 0 }; i < size; i++ )
		assert( test[i] == i + 1 );
}

DECLARE_OOP_TEST( full_iteration )
{
	MyDeque<int> test{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int counter = 0;
	for ( auto & x : test )
	{
		assert( x == test[counter] );
		counter++;
	}
	assert( counter == test.size() );
}

DECLARE_OOP_TEST( shrink_to_fit )
{
	MyDeque<int> test( 10 );
	assert( test.size() == 10 );

	for ( int i = 0; i < 90; i++ )
		test.push_back( 1 );
	assert( test.size() == 100 );

	test.shrink_to_fit();
	assert( test.size() == 100 );
	assert( test.capacity() == 100 );
}