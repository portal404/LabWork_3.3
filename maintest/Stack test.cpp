#include <gtest.h>
#include "StackClass.h"

TEST(TStackTest, DefaultConstructor)
{
    TStack<int> stack;
    EXPECT_EQ(stack.GetCapacity(), 10);
    EXPECT_EQ(stack.GetTop(), 0);
    EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, ParameterizedConstructor)
{
    TStack<int> stack(5);
    EXPECT_EQ(stack.GetCapacity(), 5);
    EXPECT_EQ(stack.GetTop(), 0);
    EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, CopyConstructor)
{
    TStack<int> stack1(3);
    stack1.push(1);
    stack1.push(2);

    TStack<int> stack2(stack1);
    EXPECT_EQ(stack2.GetCapacity(), 3);
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2[0], 1);
    EXPECT_EQ(stack2[1], 2);
}

TEST(TStackTest, MoveConstructor)
{
    TStack<int> stack1(2);
    stack1.push(10);
    stack1.push(20);

    TStack<int> stack2(std::move(stack1));
    EXPECT_EQ(stack2.GetCapacity(), 2);
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2[0], 10);
    EXPECT_EQ(stack2[1], 20);
    EXPECT_EQ(stack1.GetMemory(), nullptr);
}

TEST(TStackTest, PushAndPop)
{
    TStack<int> stack(2);
    stack.push(1);
    stack.push(2);
    
    EXPECT_EQ(stack.Size(), 2);
    EXPECT_TRUE(stack.IsFull());
    
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.pop(), 1);
    EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, AutoResize)
{
    TStack<int> stack(2);
    stack.push(1);
    stack.push(2);
    stack.push(3);

    EXPECT_GE(stack.GetCapacity(), 3);
    EXPECT_EQ(stack.Size(), 3);
    EXPECT_EQ(stack[2], 3);
}

TEST(TStackTest, PopEmptyThrows)
{
    TStack<int> stack;
    EXPECT_THROW(stack.pop(), const char*);
}

TEST(TStackTest, IndexOutOfRangeThrows)
{
    TStack<int> stack;
    EXPECT_THROW(stack[0], const char*);
    
    stack.push(1);
    EXPECT_THROW(stack[1], const char*);
}

TEST(TStackTest, SetCapacityValid)
{
    TStack<int> stack(5);
    stack.push(1);
    stack.push(2);
    
    stack.SetCapacity(10);
    EXPECT_EQ(stack.GetCapacity(), 10);
    EXPECT_EQ(stack.Size(), 2);
}

TEST(TStackTest, SetCapacityInvalidThrows)
{
    TStack<int> stack(5);
    stack.push(1);
    stack.push(2);
    stack.push(3);
    
    EXPECT_THROW(stack.SetCapacity(2), const char*);
}

TEST(TStackTest, EqualityOperator)
{
    TStack<int> stack1(3);
    stack1.push(1);
    stack1.push(2);
    
    TStack<int> stack2(3);
    stack2.push(1);
    stack2.push(2);
    
    EXPECT_TRUE(stack1 == stack2);
    
    stack2.push(3);
    EXPECT_FALSE(stack1 == stack2);
}

TEST(TStackTest, Iterator)
{
    TStack<int> stack(3);
    stack.push(1);
    stack.push(2);
    stack.push(3);
    int expected[] = {1, 2, 3};
    int i = 0;
    for (auto it = stack.begin(); it != stack.end(); ++it)
    {
        EXPECT_EQ(*it, expected[i++]);
    }
}

TEST(TStackTest, IteratorPostIncrement)
{
    TStack<int> stack(2);
    stack.push(10);
    stack.push(20);
    
    auto it = stack.begin();
    EXPECT_EQ(*it++, 10);
    EXPECT_EQ(*it, 20);
}

TEST(TStackTest, SetTopValid)
{
    TStack<int> stack(5);
    stack.push(1);
    stack.push(2);
    
    stack.SetTop(1);
    EXPECT_EQ(stack.Size(), 1);
    EXPECT_EQ(stack.pop(), 1);
}

TEST(TStackTest, SetTopInvalidThrows)
{
    TStack<int> stack(2);
    EXPECT_THROW(stack.SetTop(3), const char*);
}

TEST(TStackTest, FullEmptyConditions)
{
    TStack<int> stack(2);
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_FALSE(stack.IsFull());
    
    stack.push(1);
    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_FALSE(stack.IsFull());
    
    stack.push(2);
    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_TRUE(stack.IsFull());
}