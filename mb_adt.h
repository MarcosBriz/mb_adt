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
};

template<class T>
class List {
 public:
  List() { count_ = 0; head_ = nullptr; tail_ = nullptr; }
  List(const List &other) { count_ = other.count_; head_ = other.head_; tail_ = other.tail_; }
  ~List() { clear(); }
  
  void add(const T &item)
  {
    Node<T> *new_entry = new Node<T>();
    
    new_entry->data = item;
    
    addInternal(new_entry);
  }  
  
  void add(T &&item)
  {
    Node<T> *new_entry = new Node<T>();
    
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
    
    Node<T> *new_entry = new Node<T>();
    
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
    
    Node<T> *new_entry = new Node<T>();
    
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
    
    Node<T> *iterator;
    
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
    
    Node<T> *iterator = head_;
    
    for(uint32_t i = 0; i < index-1; ++i) { iterator = iterator->next; }
    
    removeNextNode(iterator);
  }
  
  void clear()
  {
    if(count_ == 0)
      return;
    
    Node<T> *to_remove = head_;
    
    for(Node<T> *iterator = head_->next; iterator != nullptr; iterator = iterator->next)
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
    
    return *this;
  };
  
  void printList()
  {
    printf("\n\nList Debug Print %p\n", this);
    printf("\nItem count: %d\nHead: %p, Tail: %p\n", count_, head_, tail_);
    
    for(Node<T> *iterator = head_; iterator != nullptr; iterator = iterator->next)
    {
      printf("\nNode %p, next %p, data %p", iterator, iterator->next, &(iterator->data));
    }
    
  }
  
 private:
  uint32_t count_;
  Node<T> *head_;
  Node<T> *tail_;
 
  void removeNextNode(Node<T> *node)
  {
    Node<T> *to_remove = node->next;
    
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
    Node<T> *next = head_->next;
    head_->next = nullptr;
    delete head_;
      
    head_ = next;
    --count_;
      
    if(count_ == 0)
      tail_ = nullptr;
  }
  
  void addInternal(Node<T> *new_entry)
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
  
  void insertInternal(Node<T> *new_entry, int32_t position)
  {
    
    if(position == 0)
    {
      new_entry->next = head_;
      head_ = new_entry;
    }
    else
    {
      Node<T> *iterator = head_;
      
      for(uint32_t i = 0; i < position - 1; ++i) { iterator = iterator->next; }
      
      Node<T> *next = iterator->next;
      iterator->next = new_entry;
      new_entry->next = next;
    }
    
    ++count_;
  }
 
};


};

#endif