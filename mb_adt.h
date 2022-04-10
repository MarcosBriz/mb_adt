//Author: Marcos Briz Rangel

#ifndef __MB_ADT_H__
#define __MB_ADT_H__ 1

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <utility>

namespace mb
{

template<typename T>
struct Node {
  T data;
  Node<T> *next;
  Node<T> *prev;
};

template<class T>
class List {
 public:
  List() { count_ = 0; head_ = nullptr; tail_ = nullptr; mem_allocs_ = 0; }
  List(const List &other) { count_ = other.count_; head_ = other.head_; tail_ = other.tail_; mem_allocs_ = other.mem_allocs_; }
  ~List() { clear(); assert(mem_allocs_ == 0); }
  
  void add(const T &item)
  {
    Node<T> *new_entry = new Node<T>(); ++mem_allocs_;
    
    new_entry->data = item;
    
    addInternal(new_entry);
  }  
  
  void add(T &&item)
  {
    Node<T> *new_entry = new Node<T>(); ++mem_allocs_;
    
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
    
    Node<T> *new_entry = new Node<T>(); ++mem_allocs_;
    
    new_entry->data = item;
    
    insertInternal(new_entry);
  }  
  
  void insert(T &&item, uint32_t position)
  {
    if(position >= count_) //Adds item at last position.
    {
      add(std::move(item));
      return;
    }
    
    Node<T> *new_entry = new Node<T>(); ++mem_allocs_;
    
    new_entry->data = std::move(item);
    
    insertInternal(new_entry);
  }
  
  void remove(const T &item)
  {
    Node<T> *iterator;
    
    for(iterator = head_; iterator != nullptr && iterator->data != item; iterator = iterator->next);
    
    if(iterator == nullptr)
      return;
    
    removeNode(iterator);
  }
  
  void removeAt(uint32_t index)
  {
    if(index >= count_)
      return;
    
    Node<T> *iterator = head_;
    
    for(uint32_t i = 0; i < index; ++i) { iterator = iterator->next; }
    
    removeNode(iterator);
  }
  
  void clear()
  {
    if(count_ == 0)
      return;
    
    for(Node<T> *iterator = tail_->prev; iterator != nullptr; iterator = iterator->prev)
    {
      Node<T> *to_remove = iterator->next;
      
      to_remove->prev = nullptr;
      to_remove->next = nullptr;
      
      delete to_remove; --mem_allocs_;
    }
    
    head_->prev = nullptr;
    head_->next = nullptr;
    
    delete head_; --mem_allocs_;
    
    count_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
  }
  
  void traverse(void(*func)(T&))
  {
    for(Node<T> *iterator = head_; iterator != nullptr; iterator = iterator->next)
    {
      func(iterator->data);
    }
  }
  
  bool contains(const T &item)
  {
    bool found = false;
    
    for(Node<T> *iterator = head_; iterator != nullptr && !found; iterator = iterator->next)
    {
      found = iterator->data == item;
    }
    
    return found;
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
    mem_allocs_ = other.mem_allocs_;
    
    return *this;
  };
  
  void printList()
  {
    printf("\n\nList Debug Print %p\n", this);
    printf("\nItem count: %d\nHead: %p, Tail: %p\n", count_, head_, tail_);
    
    for(Node<T> *iterator = head_; iterator != nullptr; iterator = iterator->next)
    {
      printf("\nNode %p, prev %p, next %p, data %p", iterator, iterator->prev, iterator->next, &(iterator->data));
    }
    
  }
  
 private:
  uint32_t count_;
  Node<T> *head_;
  Node<T> *tail_;
  
  uint32_t mem_allocs_;
 
  void removeNode(Node<T> *node)
  {
    if(node->next != nullptr)
    {
      node->next->prev = node->prev;
      
      if(node->prev == nullptr)
        head_ = node->next;
    }
    
    if(node->prev != nullptr)
    {
      node->prev->next = node->next;
      
      if(node->next == nullptr)
        tail_ = node->prev;
    }
   
    node->prev = nullptr;
    node->next = nullptr;
    
    delete node; --mem_allocs_;
    
    --count_;
    
    if(count_ == 0)
    {
      head_ = nullptr;
      tail_ = nullptr;
    }
  }
  
  void addInternal(Node<T> *new_entry){
    
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
  
  void insertInternal(Node<T> *new_entry){
    
    Node<T> *iterator = head_;
    
    for(uint32_t i = 0; i < position; ++i) { iterator = iterator->next; }
    
    new_entry->prev = iterator->prev;
    new_entry->next = iterator;
    iterator->prev = new_entry;
    
    if(iterator->prev != nullptr)
      iterator->prev->next = new_entry;
    else
      head_ = new_entry;
    
    ++count_;
  }
 
};


};

#endif