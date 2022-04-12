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
  //TODO: copy constructors
  List(const List &other) { count_ = other.count_; head_ = other.head_; tail_ = other.tail_; }
  ~List() { clear(); }
  
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
  
  const List<T>& operator=(const List<T>& other) 
  {
    count_ = other.count_; 
    head_ = other.head_; 
    tail_ = other.tail_;
    
    return *this;
  };
  
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
  
  void insertInternal(Node *new_entry, int32_t position)
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
 
};

template<class T>
class LockeableList : public List<T>, public ThreadLocks { };

template<class T>
class Array {

 public:
  
  Array()
  {
    data_ = nullptr;
    lenght_ = 0;
  }
 
  Array(int32_t size) 
  {
    data_ = new T[size];
    lenght_ = size;
  }
  
  ~Array() 
  { 
    if(data_ != nullptr) { delete[] data_; }
    lenght_ = 0;
  }
  //TODO: these constructors + move
  Array(const Array& other) = delete;
  const Array<T>& operator=(const Array<T>& other) = delete;
  
  void resize(int32_t size)
  {
    T* new_buffer = new T[size];
    
    int32_t cpy_lenght = size < lenght_ ? size : lenght_;

    std::memcpy(new_buffer, data_, sizeof(T) * cpy_lenght);
    std::memset(data_, 0, sizeof(T) * cpy_lenght);
    
    delete[] data_;
    data_ = new_buffer;
    lenght_ = size;
  }
  
  void memset(int8_t value) { std::memset(data_, value, sizeof(T) * lenght_); }
  
  void traverse(void(*func)(T&))
  {
    for(int32_t i = 0; i < lenght_; ++i)
    {
      func(data_[i]);
    }
  }
  
  int32_t lenght(){ return lenght_; }
  
  T& operator[](int32_t index)
  {     
    return data_[index];
  }

 private:
  int32_t lenght_;
  T *data_; 
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