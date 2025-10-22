#pragma once
#include <cstddef>

using namespace std;

//
template <class T>
class TQueue
{
protected:
    size_t capacity;
    size_t start;
    size_t finish;
    T* memory;
public:
    TQueue();
    TQueue(size_t capacity_);
    TQueue(const TQueue& other);
    TQueue(TQueue&& other);
    ~TQueue();

    size_t GetCapacity() const;
    size_t GetStart() const;
    size_t GetFinish() const;
    T* GetMemory() const;
    void SetCapacity(size_t capacity_);
    void SetStart(size_t start_);
    void SetFinish(size_t finish_);
    void SetMemory(T* memory_);

    // Размер очереди
    size_t Size() const;

    bool operator==(const TQueue<T>& other) const;
    bool operator!=(const TQueue<T>& other) const;
    T operator[](size_t index) const;

    void push(const T& element);
    T pop();
    bool IsEmpty() const;
    bool IsFull() const;

    class TIterator
    {
    protected:
        TQueue<T>& p;
        size_t current;
        size_t passed;
    public:
        TIterator(TQueue<T> &queue, size_t start_pos, size_t passed_count);
        T& operator*();
        TIterator& operator++();
        TIterator operator++(int);
        bool operator==(const TIterator& other) const;
        bool operator!=(const TIterator& other) const;
    };

    TIterator begin();
    TIterator end();

};

template <class T>
inline TQueue<T>::TQueue() : capacity(0), start(0), finish(0), memory(new T[capacity]) {}

template <class T>
inline TQueue<T>::TQueue(size_t capacity_)
{
    capacity = capacity_;
    start = 0;
    finish = 0;
    if (capacity_ > 0)
        memory = new T[capacity_];
    else
        memory = nullptr;
}

template <class T>
inline TQueue<T>::TQueue(const TQueue& other)
{
    capacity = other.capacity;
    start = other.start;
    finish = other.finish;
    if (other.capacity > 0)
      {
      memory = new T[other.capacity];
      if (other.memory)
      {
        for (size_t i = 0; i < capacity; ++i)
          memory[i] = other.memory[i];
      }
    }
    else memory = nullptr;
}

template <class T>
inline TQueue<T>::TQueue(TQueue&& other)
{
  capacity = other.capacity;
  start = other.start;
  finish = other.finish;
  memory = other.memory;

  other.capacity = 0;
  other.start = 0;
  other.finish = 0;
  other.memory = nullptr;
}

template <class T>
inline TQueue<T>::~TQueue()
{
  delete[] memory;
}

// геттеры и сеттеры

template <class T>
inline size_t TQueue<T>::GetCapacity() const
{
  return capacity;
}

template <class T>
inline size_t TQueue<T>::GetStart() const
{
  return start;
}

template <class T>
inline size_t TQueue<T>::GetFinish() const
{
  return finish;
}

template <class T>
inline T* TQueue<T>::GetMemory() const
{
  return memory;
}

template <class T>
inline void TQueue<T>::SetCapacity(size_t capacity_)
{
  if (capacity_ != capacity) {
      T* newMemory = nullptr;
      if (capacity_ > 0)
          newMemory = new T[capacity_];

      if (memory && newMemory)
      {
          size_t copySize = capacity_;
          if (capacity_ > capacity)
              copySize = capacity;

          for (size_t i = 0; i < copySize; ++i)
              newMemory[i] = memory[i];
      }
      delete[] memory;
      memory = newMemory;
      capacity = capacity_;
      if (start >= capacity)
          start = 0;
      if (finish >= capacity)
          finish = 0;
  }
}

template <class T>
inline void TQueue<T>::SetStart(size_t start_)
{
    if (start_ < capacity)
        start = start_;
}

template <class T>
inline void TQueue<T>::SetFinish(size_t finish_)
{
    if (finish_ < capacity)
        finish = finish_;
}

template <class T>
inline void TQueue<T>::SetMemory(T* memory_)
{
    memory = memory_;
    delete[] memory_;
}



template <class T>
inline size_t TQueue<T>::Size() const
{
    if (start <= finish) {
        return finish - start;
    } else {
        return capacity - start + finish;
    }
}
// Операторы

template <class T>
inline T TQueue<T>::operator[](size_t index) const
{
    if (memory == nullptr)
        throw "Queue memory is not allocated";

    if (index >= capacity)
        throw "Index out of range";

    size_t resIndex = (start + index) % capacity;
    return memory[resIndex];
}

template <class T>
inline bool TQueue<T>::operator==(const TQueue<T>& other) const
{
    if (this == &other)
        return true;

    if (capacity != other.capacity || start != other.start || finish != other.finish)
        return false;

    if (memory == other.memory)
        return true;

    for (size_t i = 0; i < capacity; ++i) {
        if (memory[i] != other.memory[i])
            return false;
    }
    return true;
}

template <class T>
inline bool TQueue<T>::operator!=(const TQueue<T>& other) const
{
    return !(*this == other);
}

// паша поп

template <class T>
inline void TQueue<T>::push(const T& element)
{
    if (IsFull()) throw "Queue is full";
    memory[finish] = element;
    finish = (finish + 1) % capacity;
}

template <class T>
inline T TQueue<T>::pop()
{
    if (IsEmpty()) throw "Queue is empty";
    T element = memory[start];
    start = (start + 1) % capacity;
    return element;
}

template <class T>
inline bool TQueue<T>::IsEmpty() const
{
    return start == finish;
}

template <class T>
inline bool TQueue<T>::IsFull() const
{
    return (finish + 1) % capacity == start;
}

// итератор

template <class T>
inline TQueue<T>::TIterator::TIterator(TQueue<T>& queue, size_t start_pos, size_t passed_count)
    : p(queue), current(start_pos), passed(passed_count) {}


template <class T>
inline T& TQueue<T>::TIterator::operator*()
{
    return p.memory[current];
}

template <class T>
inline typename TQueue<T>::TIterator& TQueue<T>::TIterator::operator++()
{
    if (passed >= p.Size()) {
        throw "Iterator out of range";
    }
    current = (current + 1) % p.capacity;
    passed++;
    return *this;
}

template <class T>
inline typename TQueue<T>::TIterator TQueue<T>::TIterator::operator++(int)
{
    TIterator temp = *this;
    ++(*this);
    return temp;
}

template <class T>
inline bool TQueue<T>::TIterator::operator==(const TIterator& other) const
{
    return &p == &other.p && current == other.current && passed == other.passed;
}

template <class T>
inline bool TQueue<T>::TIterator::operator!=(const TIterator& other) const
{
    return !(*this == other);
}

template <class T>
inline typename TQueue<T>::TIterator TQueue<T>::begin() {
    return TIterator(*this, start, 0);
}

template <class T>
inline typename TQueue<T>::TIterator TQueue<T>::end() {
    return TIterator(*this, (start + Size()) % capacity, Size());
}
