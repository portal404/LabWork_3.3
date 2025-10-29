#include <gtest.h>
#include "QueueClass.h"

TEST(TQueueTest, DefaultConstructor)
{
    TQueue<int> queue;
    EXPECT_EQ(queue.GetCapacity(), 0);
    EXPECT_EQ(queue.GetStart(), 0);
    EXPECT_EQ(queue.GetFinish(), 0);
    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_EQ(queue.Size(), 0);
}

TEST(TQueueTest, ParameterizedConstructor)
{
    TQueue<int> queue(10);
    EXPECT_EQ(queue.GetCapacity(), 10);
    EXPECT_EQ(queue.GetStart(), 0);
    EXPECT_EQ(queue.GetFinish(), 0);
    EXPECT_NE(queue.GetMemory(), nullptr);
    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_FALSE(queue.IsFull());
}

TEST(TQueueTest, CopyConstructor)
{
    TQueue<int> queue1(5);
    queue1.push(1);
    queue1.push(2);
    queue1.push(3);

    TQueue<int> queue2(queue1);
    EXPECT_EQ(queue2.GetCapacity(), 5);
    EXPECT_EQ(queue2.Size(), 3);
    EXPECT_EQ(queue2[0], 1);
    EXPECT_EQ(queue2[1], 2);
    EXPECT_EQ(queue2[2], 3);
}

TEST(TQueueTest, MoveConstructor)
{
    TQueue<int> queue1(5);
    queue1.push(1);
    queue1.push(2);

    TQueue<int> queue2(std::move(queue1));
    EXPECT_EQ(queue2.GetCapacity(), 5);
    EXPECT_EQ(queue2.Size(), 2);
    EXPECT_EQ(queue2[0], 1);
    EXPECT_EQ(queue2[1], 2);
    EXPECT_EQ(queue1.GetCapacity(), 0);
    EXPECT_EQ(queue1.GetMemory(), nullptr);
}

TEST(TQueueTest, PushAndSize)
{
    TQueue<int> queue(5);
    queue.push(1);
    EXPECT_EQ(queue.Size(), 1);
    queue.push(2);
    EXPECT_EQ(queue.Size(), 2);
    queue.push(3);
    EXPECT_EQ(queue.Size(), 3);
}

TEST(TQueueTest, PushAndPop)
{
    TQueue<int> queue(5);
    queue.push(10);
    queue.push(20);
    queue.push(30);

    EXPECT_EQ(queue.pop(), 10);
    EXPECT_EQ(queue.Size(), 2);
    EXPECT_EQ(queue.pop(), 20);
    EXPECT_EQ(queue.Size(), 1);
    EXPECT_EQ(queue.pop(), 30);
    EXPECT_EQ(queue.Size(), 0);
    EXPECT_TRUE(queue.IsEmpty());
}

TEST(TQueueTest, IsEmptyAndIsFull)
{
    TQueue<int> queue(3);
    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_FALSE(queue.IsFull());

    queue.push(1);
    EXPECT_FALSE(queue.IsEmpty());
    EXPECT_FALSE(queue.IsFull());

    queue.push(3);
    EXPECT_FALSE(queue.IsEmpty());
    EXPECT_TRUE(queue.IsFull());
}

// Тесты для операторов
TEST(TQueueTest, EqualityOperator)
{
    TQueue<int> queue1(5);
    queue1.push(1);
    queue1.push(2);

    TQueue<int> queue2(5);
    queue2.push(1);
    queue2.push(2);

    TQueue<int> queue3(5);
    queue3.push(3);
    queue3.push(4);

    EXPECT_TRUE(queue1 == queue2);
    EXPECT_FALSE(queue1 == queue3);
}

TEST(TQueueTest, InequalityOperator)
{
    TQueue<int> queue1(5);
    queue1.push(1);

    TQueue<int> queue2(5);
    queue2.push(2);

    EXPECT_TRUE(queue1 != queue2);
    EXPECT_FALSE(queue1 != queue1);
}

TEST(TQueueTest, IndexOperator)
{
    TQueue<int> queue(5);
    queue.push(10);
    queue.push(20);
    queue.push(30);

    EXPECT_EQ(queue[0], 10);
    EXPECT_EQ(queue[1], 20);
    EXPECT_EQ(queue[2], 30);
}

TEST(TQueueTest, SetAndGetCapacity)
{
    TQueue<int> queue(5);
    queue.push(1);
    queue.push(2);

    queue.SetCapacity(10);
    EXPECT_EQ(queue.GetCapacity(), 10);
    EXPECT_EQ(queue.Size(), 2);
    EXPECT_EQ(queue[0], 1);
    EXPECT_EQ(queue[1], 2);
}

TEST(TQueueTest, SetStartAndFinish)
{
    TQueue<int> queue(5);
    queue.SetStart(2);
    queue.SetFinish(4);

    EXPECT_EQ(queue.GetStart(), 2);
    EXPECT_EQ(queue.GetFinish(), 4);
}

TEST(TQueueTest, IteratorBasic)
{
    TQueue<int> queue(5);
    queue.push(1);
    queue.push(2);
    queue.push(3);

    auto it = queue.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    it++;
    EXPECT_EQ(*it, 3);
}

TEST(TQueueTest, IteratorRangeBasedFor)
{
    TQueue<int> queue(5);
    queue.push(1);
    queue.push(2);
    queue.push(3);

    int sum = 0;
    for (auto& item : queue) {
        sum += item;
    }
    EXPECT_EQ(sum, 6);
}

TEST(TQueueTest, IteratorBeginEnd)
{
    TQueue<int> queue(5);
    queue.push(1);
    queue.push(2);

    auto begin = queue.begin();
    auto end = queue.end();

    EXPECT_TRUE(begin != end);
    EXPECT_FALSE(begin == end);
}

TEST(TQueueTest, IteratorModification)
{
    TQueue<int> queue(5);
    queue.push(1);
    queue.push(2);

    for (auto it = queue.begin(); it != queue.end(); ++it) {
        *it *= 2;
    }

    EXPECT_EQ(queue[0], 2);
    EXPECT_EQ(queue[1], 4);
}

TEST(TQueueTest, StringType)
{
    TQueue<std::string> queue(3);
    queue.push("hello");
    queue.push("world");

    EXPECT_EQ(queue.Size(), 2);
    EXPECT_EQ(queue[0], "hello");
    EXPECT_EQ(queue[1], "world");
    EXPECT_EQ(queue.pop(), "hello");
}

TEST(TQueueTest, DoubleType)
{
    TQueue<double> queue(4);
    queue.push(1.1);
    queue.push(2.2);
    queue.push(3.3);

    EXPECT_DOUBLE_EQ(queue[0], 1.1);
    EXPECT_DOUBLE_EQ(queue[1], 2.2);
    EXPECT_DOUBLE_EQ(queue[2], 3.3);
}