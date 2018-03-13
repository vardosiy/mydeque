#include "mydeque.hpp"
#include "testlib.hpp"
#include <deque>

DECLARE_OOP_TEST(default_constructor)
{
	std::string a;
   MyDeque<int> test;
   assert(test.size() == 0);
   assert(test.empty());

   MyDeque<double> test2;
   assert(test2.size() == 0);
   assert(test2.empty());

   MyDeque<float> test3;
   assert(test3.size() == 0);
   assert(test3.empty());

   MyDeque<std::string> test4;
   assert(test3.size() == 0);
   assert(test4.empty());
}

DECLARE_OOP_TEST(constructor_with_number_of_elements_and_their_value)
{
   MyDeque<int> test1(10, 0);
   assert(test1.size() == 10);
   for (int i = 0; i < 10; i++)
      assert(test1[i] == 0);

   MyDeque<int> test2(20, 3);
   assert(test2.size() == 20);
   for (int i = 0; i < 20; i++)
      assert(test2[i] == 3);

   MyDeque<std::string> test3(20, "hello");
   for (int i = 0; i < 20; i++)
      assert(test3[i] =="hello");
}

DECLARE_OOP_TEST(initializer_list_constructor)
{
   MyDeque<int> test1({ 1,2,3,4,5,6,7,8,9,10 });
   assert(test1.size() == 10);
   for (int i = 0; i < 10; i++)
      assert(test1[i] == i + 1);

   MyDeque<std::string> test2({"hello", "guys"});
   assert(test2[0] == "hello");
   assert(test2[1] == "guys");
}

DECLARE_OOP_TEST(copy_constructor_with_same_type)
{
   MyDeque<int> test1({ 1,2,3,4,5,6,7,8,9,10 });
   MyDeque<int> test2(test1);

   for (int i = 0; i < 10; i++)
      assert(test2[i] == test1[i]);
   assert(test1.size() == test2.size());

   MyDeque<std::string> test3({ "hello", "guys" });
   MyDeque<std::string> test4({ "new", "strings" });
   test3 = test4;
   assert(test3[0] == "new");
   assert(test3[1] == "strings");
}

DECLARE_OOP_TEST(copy_constructor_with_different_type)
{
   MyDeque<double> test1({ 9.9, 8.8, 7.7, 6.6, 5.5, 4.4, 3.3, 2.2, 1.1 });
   MyDeque<int> test2(test1);

   for (int i = 0; i < 9; i++)
      assert(test2[i] == (int)(test1[i]));

   assert(test1.size() == test2.size());
}

DECLARE_OOP_TEST(compare_operators)
{
   MyDeque<int> test1({ 1,2,3,4,5,6,7,8,9 });
   MyDeque<int> test2({ 9,8,7,6,5,4,3,2,1 });
   MyDeque<int> test3({ 1,2,3,4,5,6,7,8,9 });
   MyDeque<int> test4({ 1,2,3,4,5,6,7,8,9,10 });
   MyDeque<std::string> test5({ "hello", "guys" });
   MyDeque<std::string> test6({ "new", "strings" });

   assert(!(test1 == test2));
   assert(test1 != test2);
   assert(test1 == test3);
   assert(test1 != test4);
   assert(!(test5 == test6));
   assert(test5 != test6);
}

DECLARE_OOP_TEST(appropriation_operator_with_different_types)
{
   MyDeque<int> test1({ 1,2,3,4,5,6,7,8,9 });
   MyDeque<double> test2({ 9.9, 8.8, 7.7, 6.6, 5.5, 4.4, 3.3, 2.2, 1.1 });
   test1 = test2;
   for (int i = 0; i < 9; i++)
      assert(test1[i] == (int)test2[i]);
}

DECLARE_OOP_TEST(appropriation_operator_with_same_types_and_check_for_creating_correct_copy)
{
   MyDeque<int> test1({ 9,8,7,6,5,4,3,2,1 });
   {
      MyDeque<int> test2({ 1,2,3,4,5,6,7,8,9 });
      test1 = test2;
   }
   for (int i = 0; i < 9; i++)
      assert(test1[i] == i + 1);
}

DECLARE_OOP_TEST(clear_and_empty)
{
   MyDeque<int> test1(5);
   assert(test1.size() == 5);
   assert(!test1.empty());
   test1.clear();
   assert(test1.empty());

   MyDeque<int> test2(15);
   assert(test2.size() == 15);
   assert(!test2.empty());
   test2.clear();
   assert(test2.empty());

   MyDeque<int> test3;
   test3.push_back(1);
   assert(test3.size() == 1);
   test3.pop_back();
   assert(test3.empty());

   MyDeque<std::string> test4;
   test4.push_back("some string");
   assert(test4.size() == 1);
   test4.pop_back();
   assert(test4.empty());
}

DECLARE_OOP_TEST(size)
{
   MyDeque<int> test(8);
   assert(test.size() == 8);
   test.push_back(1);
   assert(test.size() == 9);
   test.push_back(1);
   assert(test.size() == 10);
   test.push_front(1);
   assert(test.size() == 11);
   test.push_front(1);
   assert(test.size() == 12);
   test.pop_back();
   test.pop_front();
   assert(test.size() == 10);
}

DECLARE_OOP_TEST(const_at)
{
   MyDeque<int> test({ 0,1,2,3,4,5 });
   
   for (int i = 0; i <= 5; i++)
      assert(test.at(i) == i);
   try
   {
      test.at(6);
      assert(!"Exception must have been thrown");
   }
   catch (std::exception & e)
   {
      assert(!strcmp(e.what(), Messages::InvalidIndex));
   }

   MyDeque<std::string> test2({ "some", "strings", "for", "testing" });
   assert(test2.at(0) == "some");
   assert(test2.at(1) == "strings");
   assert(test2.at(2) == "for");
   assert(test2.at(3) == "testing");
   try
   {
      test2.at(4);
      assert(!"Exception must have been thrown");
   }
   catch (std::exception & e)
   {
      assert(!strcmp(e.what(), Messages::InvalidIndex));
   }
}

DECLARE_OOP_TEST(at_for_writing)
{
   MyDeque<int> test({ 0,1,2,3,4,5 });

   for (int i = 0; i <= 5; i++)
      test.at(i) = i + 10;

   for (int i = 0; i <= 5; i++)
      assert(test.at(i) == i + 10);
   try
   {
      test.at(6);
      assert(!"Exception must have been thrown");
   }
   catch (std::exception & e)
   {
      assert(!strcmp(e.what(), Messages::InvalidIndex));
   }

   MyDeque<std::string> test2(4, "something");
   for (int i = 0; i < 4; i++)
      test2.at(i) = "new str";

   for (int i = 0; i < 4; i++)
      assert(test2.at(i) == "new str");
   try
   {
      test2.at(5);
      assert(!"Exception must have been thrown");
   }
   catch (std::exception & e)
   {
      assert(!strcmp(e.what(), Messages::InvalidIndex));
   }
}

DECLARE_OOP_TEST(index_operator)
{
   MyDeque<int> test({ 0,1,2,3,4,5 });
   for (int i = 0; i < 6; i++)
      assert(test[i] == i);
   test.push_front(1);
   assert(test[0] == 1);
   test.push_back(1);
   assert(test[test.size() - 1] == 1);

   MyDeque<std::string> test2({ "some", "strings", "for", "testing" });
   assert(test2[0] == "some");
   assert(test2[1] == "strings");
   assert(test2[2] == "for");
   assert(test2[3] == "testing");
}

DECLARE_OOP_TEST(writing_by_index_operator)
{
   MyDeque<int> test(10);
   for (int i = 0; i < 10; i++)
      test[i] = 15;
   for (int i = 0; i < 10; i++)
      assert(test[i] == 15);

   MyDeque<std::string> test2(4, "something");
   for (int i = 0; i < 4; i++)
      test2[i] = "new str";
   for (int i = 0; i < 4; i++)
      assert(test2[i] == "new str");
}

DECLARE_OOP_TEST(front_and_back_const)
{
   MyDeque<int> test(1);
   assert(test.front() == test.back());
   test.push_back(1);
   assert(test.back() == 1);
   test.push_front(9);
   assert(test.front() == 9);

   MyDeque<std::string> test2({ "some", "strings", "for", "testing" });
   assert(test2.front() == "some");
   assert(test2.back() == "testing");
}

DECLARE_OOP_TEST(front_and_back_with_writing)
{
   MyDeque<int> test(3,0);
   test.front() = 1;
   assert(test[0] == 1);
   test.back() = 2;
   assert(test[test.size() - 1] == 2);

   MyDeque<std::string> test2({ "some", "strings", "for", "testing" });
   test2.front() = "new";
   test2.back() = "str";
   assert(test2.front() == "new");
   assert(test2.back() == "str");
}

DECLARE_OOP_TEST(pushBack_and_popBack)
{
	MyDeque< std::vector<int> > test;

	test.push_back({});
   assert(test.size() == 1);
   test.pop_back();
   assert(test.size() == 0);
   for (int i = 1; i <= 100; i++)
      test.push_back({});
   assert(test.size() == 100);
   for (int i = 0; i < 10; i++)
      test.pop_back();
   assert(test.size() == 90);
   for (int i = 0; i < 10; i++)
      test.pop_back();
   assert(test.size() == 80);
   for (int i = 0; i < 20; i++)
      test.push_back({});
   assert(test.size() == 100);
}

DECLARE_OOP_TEST(pushFront_and_popFront)
{
   MyDeque<int> test;

   test.push_front(1);
   assert(test.size() == 1);
   test.pop_front();
   assert(test.size() == 0);

   for (int i = 1; i <= 100; i++)
      test.push_front(i);
   assert(test.size() == 100);

   for (int i = 0; i < 10; i++)
      test.pop_front();
   assert(test.size() == 90);

   for (int i = 0; i < 10; i++)
      test.pop_front();
   assert(test.size() == 80);

   for (int i = 0; i < 20; i++)
      test.push_front(i);
   assert(test.size() == 100);
}

DECLARE_OOP_TEST(pushBack_and_popFront)
{
   MyDeque<int> test;
   
   test.push_back(10);
   test.pop_front();
   assert(test.empty());

   for(int i = 0; i < 10; i++)
      test.push_back(10);

   for (int i = 0; i < 10; i++)
      test.pop_front();

   assert(test.empty());
}

DECLARE_OOP_TEST(pushFront_and_popBack)
{
   MyDeque<int> test;

   test.push_front(10);
   test.pop_back();
   assert(test.empty());

   for (int i = 0; i < 10; i++)
      test.push_front(10);

   for (int i = 0; i < 10; i++)
      test.pop_back();

   assert(test.empty());
}

DECLARE_OOP_TEST(iterator_correct_values)
{
   MyDeque<int> test({ 9,8,7,6,5,4,3,2,1 });
   auto it = test.begin();
   for (int i = 0; i < 9; i++)
   {
      assert(*it == test[i]);
      ++it;
   }
}

DECLARE_OOP_TEST(check_iterator)
{
   MyDeque<int> test({ 9,8,7,6,5,4,3,2,1 });
   auto it1 = test.begin();
   auto it2 = test.begin();
   assert(it1 == it2);
   assert(!(it1 != it2));
   it1++;
   ++it2;
   assert(it1 == it2);
   it1 = it1 + 1;
   it2++;
   assert(it1 == it2);
}

DECLARE_OOP_TEST(writing_with_iterator)
{
   MyDeque<int> test({ 9,8,7,6,5,4,3,2,1 });
   auto it = test.begin();
   for (int i = 0; i < 9; i++)
   {
      *it = i;
      ++it;
   }
   it = test.begin();
   for (int i = 0; i < 9; i++)
   {
      assert(*it == i);
      ++it;
   }
}

DECLARE_OOP_TEST(insert_one_element)
{
   MyDeque<int> test({ 1,2,3,4,5,6,7,8,9 });
   auto it = test.begin();
   test.insert(it, 0);
   for (int i = 0; i < 10; i++)
      assert(test[i] == i);
}

DECLARE_OOP_TEST(insert_sequence)
{
   MyDeque<int> test({ 1,2,3,4,5,6,7,8,9 });
   std::vector<int> v({ 10,11,12 });
   auto it1 = v.begin();
   auto it2 = v.end();
   test.insert(it1, it2);
   for (int i = 0; i < 12; i++)
      assert(test[i] == i + 1);
}

DECLARE_OOP_TEST(erase_one_element)
{
   MyDeque<int> test({ 1,2,3,4,5,6,7,8,9 });
   assert(test.size() == 9);
   test.erase(test.begin());
   assert(test[0] == 2);
   test.erase(test.begin() + 2);
   assert(test[2] == 5);
}

DECLARE_OOP_TEST(erase_sequence)
{
   MyDeque<int> test({ 1,2,3,4,5,6,7,8,9 });
   assert(test.size() == 9);
   auto it1 = test.end();
   auto it2 = test.end();
   it1 = it1 - 2;
   test.erase(it1, it2);
   for (int i = 0; i < 6; i++)
      assert(test[i] == i + 1);
   assert(test.size() == 6);
}

DECLARE_OOP_TEST(full_iteration)
{
   MyDeque<int> test({ 1,2,3,4,5,6,7,8,9 });
   int counter = 0;
   for (auto & x : test)
   {
      assert(x == test[counter]);
      counter++;
   }
}
//
//DECLARE_OOP_TEST(incrementation_iteration)
//{
//   MyDeque<int> test({ 1,2,3,4,5,6,7,8,9 });
//   auto it1 = test.begin();
//   auto it2 = ++it1;
//   assert(it2 == it1);
//   auto it3 = it1++;
//   assert(it3 == it2);
//}
//
//DECLARE_OOP_TEST(shrink_to_fit)
//{
//   MyDeque<int> test(10);
//   assert(test.size() == 10);
//
//   for (int i = 0; i < 90; i++)
//      test.push_back(1);
//   assert(test.size() == 100);
//   
//   test.shrink_to_fit();
//   assert(test.size() == 100);
//}