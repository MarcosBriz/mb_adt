//Author: Marcos Briz Rangel

#ifndef __MB_ADT_H__
#define __MB_ADT_H__ 1

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <utility>
#include <mutex>
#include <atomic>

namespace mb
{
  
template<class T>
class List {
  
 public:
 
  List() { count_ = 0; head_ = nullptr; tail_ = nullptr; }
  
  ~List() { clear(); }
  
  List(const List &other) 
  { 
    copy(other);
  }
    
  const List<T>& operator=(const List<T>& other) 
  {
    copy(other);
  
    return *this;
  }
    
  List(List &&other) 
  { 
    move(std::move(other));
  }
  
  const List<T>& operator=(List<T>&& other) 
  {
    move(std::move(other));
  
    return *this;
  }
  
  void add(const T &item)
  {
    Node *new_entry = new Node();
    
    new_entry->data = item;
    
    addInternal(new_entry);
  }  
  
  void add(T &&item)
  {
    Node *new_entry = new Node();
    
    new_entry->data = std::move(item);
    
    addInternal(new_entry);
  }
  
  void insert(const T &item, uint32_t position)
  {
    if(position >= count_) //Adds item at last position.
    {
      add(item);
      return;
    }
    
    Node *new_entry = new Node();
    
    new_entry->data = item;
    
    insertInternal(new_entry, position);
  }  
  
  void insert(T &&item, uint32_t position)
  {
    if(position >= count_) //Adds item at last position.
    {
      add(std::move(item));
      return;
    }
    
    Node *new_entry = new Node();
    
    new_entry->data = std::move(item);
    
    insertInternal(new_entry, position);
  }
  
  void remove(const T &item)
  {
    if(count_ == 0)
      return;
    
    if(head_->data == item)
    {
      removeHead();
      return;
    }
    
    Node *iterator;
    
    for(iterator = head_; iterator->next != nullptr && iterator->next->data != item; iterator = iterator->next);
    
    if(iterator->next == nullptr)
      return;
    
    removeNextNode(iterator);
  }
  
  void removeAt(uint32_t index)
  {
    if(index >= count_)
      return;
    
    if(index == 0)
    {
      removeHead();
      return;
    }
    
    Node *iterator = head_;
    
    for(uint32_t i = 0; i < index-1; ++i) { iterator = iterator->next; }
    
    removeNextNode(iterator);
  }
  
  void clear()
  {
    if(count_ == 0)
      return;
    
    Node *to_remove = head_;
    
    for(Node *iterator = head_->next; iterator != nullptr; iterator = iterator->next)
    {
      
      to_remove->next = nullptr;
      delete to_remove;
      
      to_remove = iterator;
    }
    
    to_remove->next = nullptr;
    delete to_remove;
    
    count_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
  }
  
  void traverse(void(*func)(T&))
  {
    for(Node *iterator = head_; iterator != nullptr; iterator = iterator->next)
    {
      func(iterator->data);
    }
  }
  
  bool contains(const T &item)
  {
    bool found = false;
    
    for(Node *iterator = head_; iterator != nullptr && !found; iterator = iterator->next)
    {
      found = iterator->data == item;
    }
    
    return found;
  }
  
  uint32_t count() { return count_; }
  
  T& operator[](uint32_t index)
  {
    
    Node *indexed = head_;
    
    for(uint32_t i = 0; i < index && indexed != nullptr; ++i)
    {
      indexed = indexed->next;
    }
    
    assert(indexed != nullptr);
    
    return indexed->data;
  }  
  
  const List<T>& operator+=(const T &item)
  {
    add(item);
    return *this;
  }    
  
  const List<T>& operator+=(T &&item)
  {
    add(std::move(item));
    return *this;
  }  
  
  const List<T>&  operator-=(const T &item)
  {
    remove(item);
    return *this;
  }
  
  void printList()
  {
    printf("\n\nList Debug Print %p\n", this);
    printf("\nItem count: %d\nHead: %p, Tail: %p\n", count_, head_, tail_);
    
    for(Node *iterator = head_; iterator != nullptr; iterator = iterator->next)
    {
      printf("\nNode %p, next %p, data %p", iterator, iterator->next, &(iterator->data));
    }
    
  }
  
 private:
 
  struct Node {
    T data;
    Node *next;
  };
 
  uint32_t count_;
  Node *head_;
  Node *tail_;
 
  void removeNextNode(Node *node)
  {
    Node *to_remove = node->next;
    
    node->next = to_remove->next;
    
    to_remove->next = nullptr;
    
    delete to_remove;
    
    --count_;
    
    if(count_ == 0)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
    else if(node->next == nullptr)
    {
      tail_ = node;
    }
  }
  
  void removeHead()
  {
    Node *next = head_->next;
    head_->next = nullptr;
    delete head_;
      
    head_ = next;
    --count_;
      
    if(count_ == 0)
      tail_ = nullptr;
  }
  
  void addInternal(Node *new_entry)
  {
    
    if(count_ == 0)
    {
      head_ = new_entry;
      tail_ = head_;
      
      new_entry->next = nullptr;
    }
    else
    {
      new_entry->next = nullptr;
      
      tail_->next = new_entry;
      
      tail_ = new_entry;
    }
    
    ++count_;
  }
  
  void insertInternal(Node *new_entry, uint32_t position)
  {
    
    if(position == 0)
    {
      new_entry->next = head_;
      head_ = new_entry;
    }
    else
    {
      Node *iterator = head_;
      
      for(uint32_t i = 0; i < position - 1; ++i) { iterator = iterator->next; }
      
      Node *next = iterator->next;
      iterator->next = new_entry;
      new_entry->next = next;
    }
    
    ++count_;
  }
  
  void copy(const List& other)
  {
    clear();
    
    for(Node *it = other.head_; it != nullptr; it = it->next)
      add(it->data);
    
    count_ = other.count_;
  }  
  
  void move(List&& other)
  {
    clear();
    
    head_ = other.head_;
    tail_ = other.tail_;
    count_ = other.count_;
    
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.count_ = 0;
  }
  
 public:
 
  class Iterator {
   public:
    
    Iterator(){ node = nullptr; }
    Iterator(const Iterator &other){ node = other.node; }
    Iterator(Iterator &other){ node = other.node; }
    Iterator(Iterator &&other){ node = other.node; other.node = nullptr; }
    
    T& operator*()
    {
      return node->data;
    }    
    
    Iterator& operator=(const Iterator &other)
    {
      node = other.node;
      return *this;
    }        
    
    Iterator& operator=(Iterator &other)
    {
      node = other.node;
      return *this;
    }    
    
    Iterator& operator=(Iterator &&other)
    {
      node = other.node;
      other.node = nullptr;
      return *this;
    }    
    
    bool operator==(const Iterator &other)
    {
      return node == other.node;
    }    
    
    bool operator!=(const Iterator &other)
    {
      return node != other.node;
    }    
    
    bool operator==(const void *p)
    {
      return node == p;
    }    
    
    bool operator!=(const void *p)
    {
      return node != p;
    }
    
    Iterator& operator++()
    {
      node = node->next;
      return *this;
    }
   
   private:

    Node *node;
    
    Iterator(Node *node) { this->node = node; }
    
    friend List;
  };
  
  Iterator begin() { return Iterator(head_); }
  
  Iterator end() { return Iterator(tail_); }
};

template<class T>
class LockeableList : public List<T>, public ThreadLocks { };

template<class T>
class Vector {
  
 public:
 
  Vector() { storage_ = nullptr; capacity_ = 0; count_ = 0; }
  
  ~Vector() { destroy(); }
  
  Vector(const Vector& other)
  {
    copy(other);
  }
  
  Vector<T>& operator=(const Vector& other)
  {
    copy(other);
    return *this;
  }
  
  Vector(Vector&& other)
  {
    move(std::move(other));
  }
  
  Vector<T>& operator=(Vector&& other)
  {
    move(std::move(other));
    return *this;
  }
  
  void init(uint32_t capacity) 
  {
    destroy();
    //Purposely not calling any constructor.
    storage_ = (T*)calloc(capacity, sizeof(T));
    
    capacity_ = capacity;
    count_ = 0;
  }
  
  void destroy()
  {
    if(storage_ != nullptr)
    {
      clear();
      free(storage_);
    }
    
    storage_ = nullptr;
    capacity_ = 0;
    count_ = 0;
  }
  
  void clear()
  {
    for(uint32_t i = 0; i < count_; ++i)
    {
      storage_[i].~T();
    }
    
    memset(storage_, 0, sizeof(T) * count_);
    
    count_ = 0;
  }
  
  void resize(uint32_t new_size)
  {
    if(new_size == capacity_)
      return;
    
    T* new_storage = (T*)malloc(new_size*sizeof(T));
    
    if(new_size >= capacity_)
    {
      std::memcpy(new_storage, storage_, sizeof(T) * capacity_);
      std::memset(new_storage + capacity_, 0, sizeof(T) * (new_size - capacity_));
      memset(storage_, 0, sizeof(T) * count_);
    }
    else
    {
      std::memcpy(new_storage, storage_, sizeof(T) * new_size);
      
      for(uint32_t i = new_size; i < count_; ++i)
      {
        storage_[i].~T();
      }
      
      memset(storage_, 0, sizeof(T) * count_);
      
      count_ = new_size < count_ ? new_size : count_;
    }
    
    free(storage_);
    
    storage_ = new_storage;
    capacity_ = new_size;
  }
  
  void add(const T &item)
  {
    if(count_ >= capacity_)
      return;
    
    storage_[count_] = item;
    ++count_;
  }  
  
  void add(T &&item)
  {
    if(count_ >= capacity_)
      return;
    
    storage_[count_] = std::move(item);
    ++count_;
  }
  
  void remove(const T &item)
  {
    bool found = false;
    uint32_t i;
    
    for(i = 0; i < count_ && !found; ++i)
    {
      if(storage_[i] == item)
      {
        removeAt(i);
        found = true;
      }
    }
  }
  
  void removeAt(uint32_t index)
  {
    if(index >= count_)
      return;
    
    storage_[index].~T();
    
    uint32_t data_to_move = count_ - index - 1;
    
    memcpy(storage_ + index, storage_ + index + 1, sizeof(T) * data_to_move);
    memset(storage_ + (count_ - 1), 0, sizeof(T));
    
    --count_;
  }

  void debugPrint()
  {
    printf("\n\nVector %p, storage %p, capacity %d, count %d\n", this, storage_, capacity_, count_);
    printf("\nContent:\n\n");
    
    for(uint32_t i = 0; i < count_; ++i)
    {
      printf(" %p ", storage_ + i);
    }
  }

  uint32_t capacity() { return capacity_; }
  uint32_t count() { return count_; }
  
  Vector<T>& operator+=(const T &item)
  {
    add(item);
    return *this;
  }  
  
  Vector<T>& operator+=(T &&item)
  {
    add(std::move(item));
    return *this;
  }  
  
  Vector<T>& operator-=(const T &item)
  {
    remove(item);
    return *this;
  }
  
 private:
 
  T* storage_;
  uint32_t capacity_;
  uint32_t count_;
  
  void copy(const Vector &other)
  {
    destroy();
    
    if(other.capacity_ == 0)
      return;
    
    storage_ = (T*)malloc(sizeof(T)*other.capacity_);
    
    for(uint32_t i = 0; i < other.count_; ++i)
    {
      storage_[i] = other.storage_[i];
    }
    
    capacity_ = other.capacity_;
    count_ = other.count_;
    
    memset(storage_ + count_, 0, sizeof(T) * (capacity_ - count_));
  }
  
  void move(Vector &&other)
  {
    destroy();
    
    storage_ = other.storage_;
    capacity_ = other.capacity_;
    count_ = other.count_;
    
    other.storage_ = nullptr;
    other.capacity_ = 0;
    other.count_ = 0;
  }
  
};

template<class T>
class LockeableVector : public Vector<T>, public ThreadLocks { };

template<class T>
class Array {

 public:
  
  Array()
  {
    data_ = nullptr;
    lenght_ = 0;
  }
 
  Array(uint32_t size, bool clear_values = false) 
  {
    init(size, clear_values);
  }
  
  ~Array() 
  { 
    clear();
  }
  
  Array(const Array& other) 
  {
    copy(other);
  }
  
  const Array<T>& operator=(const Array<T>& other) 
  {
    copy(other);
    
    return *this;
  }  
  
  Array(Array&& other) 
  {
    move(std::move(other));
  }
  
  const Array<T>& operator=(Array<T>&& other) 
  {
    move(std::move(other));
  
    return *this;
  }
  
  void init(uint32_t size, bool clear_values = false)
  {
    clear();
    
    data_ = new T[size];
    lenght_ = size;
    
    if(clear_values)
      memset(0);
  }
  
  void clear()
  {
    if(data_ != nullptr)
      delete[] data_;
    lenght_ = 0;
  }
  
  void resize(uint32_t size)
  {
    if(size == lenght_)
      return;
    
    T* new_buffer = new T[size];
    
    uint32_t cpy_lenght = size < lenght_ ? size : lenght_;

    std::memcpy(new_buffer, data_, sizeof(T) * cpy_lenght);
    std::memset(data_, 0, sizeof(T) * cpy_lenght);
    
    delete[] data_;
    data_ = new_buffer;
    lenght_ = size;
  }
  
  void memset(int8_t value) { std::memset(data_, value, sizeof(T) * lenght_); }
  
  void traverse(void(*func)(T&))
  {
    for(uint32_t i = 0; i < lenght_; ++i)
    {
      func(data_[i]);
    }
  }
  
  uint32_t lenght(){ return lenght_; }
  
  T& operator[](uint32_t index)
  {     
    return data_[index];
  }

 private:
 
  uint32_t lenght_;
  T *data_; 
  
  void copy(const Array& other)
  {
    clear();
    
    data_ = new T[other.lenght_];
    lenght_ = other.lenght_;
    
    for(uint32_t i = 0; i < lenght_; ++i)
      data_[i] = other.data_[i];
  }  
  
  void move(Array&& other)
  {
    clear();
    
    data_ = other.data_;
    lenght_ = other.lenght_;
    
    other.data_ = nullptr;
    other.lenght_ = 0;
  }
  
};

template<class T>
class LockeableArray : public Array<T>, public ThreadLocks { };

class ThreadLocks {
  
 public:
 
  ThreadLocks() { spinLock_ = 0; }
  
  ThreadLocks(const ThreadLocks &other) { }
  
  ThreadLocks(ThreadLocks &&other) { }
  
  ThreadLocks& operator=(ThreadLocks&& other) {
    return *this;
  }  
  
  ThreadLocks& operator=(const ThreadLocks& other) {
    return *this;
  }

  ~ThreadLocks() {}
 
  void lock() { lock_.lock(); }
  void unlock() { lock_.unlock(); }
  
  void spinLock() {
    int i;
    while (true) {
      i = 0;
      if (spinLock_.compare_exchange_weak(i, 1)) break;
    }
  };

  void spinUnlock() { spinLock_.store(0); }
  
 private:
 
  std::mutex lock_;
  std::atomic<int> spinLock_;
};

};

#endif