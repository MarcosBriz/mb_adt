//Author: Marcos Briz Rangel

#ifndef __MB_ADT_H__
#define __MB_ADT_H__ 1

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

namespace mb
{

template<typename T>
struct Node {
  T *data;
  Node<T> *next;
  Node<T> *prev;
};

template<class T>
class List {
 public:
  List() { count_ = 0; head_ = nullptr; tail_ = nullptr; }
  List(const List &other) { count_ = other.count_; head_ = other.head_; tail_ = other.tail_; }
  ~List() { clear(); }
  
  void add(T &item)
  {
    Node<T> *new_entry = new Node<T>();
    
    new_entry->data = &item;
    
    if(count_ == 0)
    {
      head_ = new_entry;
      tail_ = head_;
      
      new_entry->next = nullptr;
      new_entry->prev = nullptr;
    }
    else
    {
      new_entry->prev = tail_;
      new_entry->next = nullptr;
      
      tail_->next = new_entry;
      
      tail_ = new_entry;
    }
    
    ++count_;
  }
  
  void insert(T &item, uint32_t position)
  {
    if(position >= count_) //Adds item at last position.
    {
      add(item);
      return;
    }
    
    Node<T> *iterator = head_;
    
    for(uint32_t i = 0; i < position; ++i)
    {
      iterator = iterator->next;
    }
    
    Node<T> *new_entry = new Node<T>();
    
    new_entry->data = &item;
    new_entry->prev = iterator->prev;
    new_entry->next = iterator;
    iterator->prev = new_entry;
    
    if(iterator->prev != nullptr)
      iterator->prev->next = new_entry;
    
    ++count_;
  }
  
  void remove(const T &item)
  {
    Node<T> *iterator;
    
    for(iterator = head_; iterator != nullptr && iterator->data != &item; iterator = iterator->next);
    
    if(iterator == nullptr)
      return;
    
    if(iterator->next != nullptr)
    {
      iterator->next->prev = iterator->prev;
      
      if(iterator->prev == nullptr)
        head_ = iterator->next;
    }
    
    if(iterator->prev != nullptr)
    {
      iterator->prev->next = iterator->next;
      
      if(iterator->next == nullptr)
        tail_ = iterator->prev;
    }
      
    iterator->data = nullptr;
    iterator->prev = nullptr;
    iterator->next = nullptr;
    
    delete iterator;
    
    --count_;
    
    if(count_ == 0)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
  }
  
  void clear()
  {
    if(count_ == 0)
      return;
    
    for(Node<T> *iterator = tail_->prev; iterator != nullptr; iterator = iterator->prev)
    {
      Node<T> *to_remove = iterator->next;
      
      to_remove->data = nullptr;
      to_remove->prev = nullptr;
      to_remove->next = nullptr;
      
      delete to_remove;
    }
    
    head_->data = nullptr;
    head_->prev = nullptr;
    head_->next = nullptr;
    
    delete head_;
    
    count_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
  }
  
  uint32_t count() { return count_; }
  
  T& operator[](uint32_t index)
  {
    
    Node<T> *indexed = head_;
    
    for(uint32_t i = 0; i < index && indexed != nullptr; ++i)
    {
      indexed = indexed->next;
    }
    
    assert(indexed != nullptr);
    
    return *(indexed->data);
  }  
  
  const List<T>& operator+=(T &item)
  {
    add(item);
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
    
    for(Node<T> *iterator = head_; iterator != nullptr; iterator = iterator->next)
    {
      printf("\nNode %p, prev %p, next %p, data %p", iterator, iterator->prev, iterator->next, iterator->data);
    }
    
  }
  
 private:
  uint32_t count_;
  Node<T> *head_;
  Node<T> *tail_;
 
};


};

#endif