/*
 * gVirtuS -- A GPGPU transparent virtualization component.
 *
 * Copyright (C) 2009-2010  The University of Napoli Parthenope at Naples.
 *
 * This file is part of gVirtuS.
 *
 * gVirtuS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gVirtuS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gVirtuS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by: Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>,
 *             Department of Applied Science
 */

/**
 * @file   Buffer.h
 * @author Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>
 * @date   Sun Oct 18 13:16:46 2009
 *
 * @brief
 *
 *
 */

#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <typeinfo>

#include <gvirtus/common/gvirtus-type.h>

#include "Communicator.h"

#define BLOCK_SIZE 4096

namespace gvirtus::communicators {
  /**
   * Buffer是用于序列化和反序列化数据的通用工具。它用于前端和后端之间的数据交换。它具有从输入流创建的功能，并通过输出流发送。
   */
/**
 * Buffer is a general purpose for marshalling and unmarshalling data. It's used
 * for exchanging data beetwen Frontend and Backend. It has the functionality to
 * be created starting from an input stream and to be sent over an output
 * stream.
 */
class Buffer {
 public:
//  size_t是一个无符号整数类型，通常用于表示内存大小
// 初始化大小为0，块大小为4096
  Buffer(size_t initial_size = 0, size_t block_size = BLOCK_SIZE);
  // 拷贝构造函数
  Buffer(const Buffer &orig);
  // 从输入流创建Buffer
  Buffer(std::istream &in);
  // 从一个字符数组创建Buffer
  Buffer(char *buffer, size_t buffer_size, size_t block_size = BLOCK_SIZE);
  virtual ~Buffer();

// 将一个元素添加到Buffer中
  template <class T>
  void Add(T item) {
    // 如果Buffer中的数据长度加上一个元素的大小大于等于Buffer的大小，那么重新分配内存
    if ((mLength + (sizeof(T))) >= mSize) {
      // 计算新的缓冲区大小
      mSize = ((mLength + (sizeof(T))) / mBlockSize + 1) * mBlockSize;
      // 重新分配内存
      // realloc 是 C 标准库中的一个函数，用于动态调整已分配内存块的大小。它可以在不丢失原有数据的情况下扩展或缩小内存块。
      if ((mpBuffer = (char *)realloc(mpBuffer, mSize)) == NULL)
        throw "Buffer::Add(item): Can't reallocate memory.";
    }
    // memmove函数用于在内存中拷贝数据，将item拷贝到mpBuffer + mLength的位置
    memmove(mpBuffer + mLength, (char *)&item, sizeof(T));
    mLength += sizeof(T);
    mBackOffset = mLength;
  }

  template <class T>
  void Add(T *item, size_t n = 1) {
    if (item == NULL) {
      Add((size_t)0);
      return;
    }
    // 记录要添加的数据的大小
    size_t size = sizeof(T) * n;
    Add(size);
    if ((mLength + size) >= mSize) {
      mSize = ((mLength + size) / mBlockSize + 1) * mBlockSize;
      if ((mpBuffer = (char *)realloc(mpBuffer, mSize)) == NULL)
        throw "Buffer::Add(item, n): Can't reallocate memory.";
    }
    memmove(mpBuffer + mLength, (char *)item, size);
    // 更新缓冲区的长度和末尾偏移量
    mLength += size;
    mBackOffset = mLength;
  }

  template <class T>
  void AddConst(const T item) {
    // 检查缓冲区是否有足够空间存储 item
    if ((mLength + (sizeof(T))) >= mSize) {
      // 计算新的缓冲区大小
      mSize = ((mLength + (sizeof(T))) / mBlockSize + 1) * mBlockSize;
      // 重新分配内存
      // realloc 是 C 标准库中的一个函数，用于动态调整已分配内存块的大小。它可以在不丢失原有数据的情况下扩展或缩小内存块。
      if ((mpBuffer = (char *)realloc(mpBuffer, mSize)) == NULL)
        throw "Buffer::AddConst(item): Can't reallocate memory.";
    }
    // 将 item 的二进制表示复制到缓冲区中
    memmove(mpBuffer + mLength, (char *)&item, sizeof(T));
    // 更新缓冲区的长度和末尾偏移量
    mLength += sizeof(T);
    mBackOffset = mLength;
  }

// 一次性拷贝n个元素到Buffer中，第一个元素为 buffer中元素的个数
  template <class T>
  void AddConst(const T *item, size_t n = 1) {
    // 检查 item 是否为空
    if (item == NULL) {
      Add((size_t)0);
      return;
    }
    // 记录要添加的数据的大小
    size_t size = sizeof(T) * n;
    Add(size);
    if ((mLength + size) >= mSize) {
      mSize = ((mLength + size) / mBlockSize + 1) * mBlockSize;
      if ((mpBuffer = (char *)realloc(mpBuffer, mSize)) == NULL)
        throw "Buffer::AddConst(item, n): Can't reallocate memory.";
    }
    memmove(mpBuffer + mLength, (char *)item, size);
    mLength += size;
    mBackOffset = mLength;
  }

  void AddString(const char *s) {
    size_t size = strlen(s) + 1;
    Add(size);
    Add(s, size);
  }

// 添加一个指针到Buffer中
// typedef uint64_t pointer_t;
// typedef __uint64_t uint64_t;
// typedef unsigned long int __uint64_t;
  template <class T>
  void AddMarshal(T item) {
    Add((gvirtus::common::pointer_t)item);
  }

  template <class T>
  void Read(Communicator *c) {
    auto required_size = mLength + sizeof(T);
    if (required_size >= mSize) {
      mSize = (required_size / mBlockSize + 1) * mBlockSize;
      if ((mpBuffer = (char *)realloc(mpBuffer, mSize)) == NULL)
        throw "Buffer::Read(*c) Can't reallocate memory.";
    }
    c->Read(mpBuffer + mLength, sizeof(T));
    mLength += sizeof(T);
    mBackOffset = mLength;
  }

  template <class T>
  void Read(Communicator *c, size_t n = 1) {
    auto required_size = mLength + sizeof(T) * n;
    if (required_size >= mSize) {
      mSize = (required_size / mBlockSize + 1) * mBlockSize;
      if ((mpBuffer = (char *)realloc(mpBuffer, mSize)) == NULL)
        throw "Buffer::Read(*c, n): Can't reallocate memory.";
    }
    c->Read(mpBuffer + mLength, sizeof(T) * n);
    mLength += sizeof(T) * n;
    mBackOffset = mLength;
  }

// Get()函数用于从Buffer中读取数据
/**
 * 这段代码实现了一个模板函数 Get<T>，
 * 用于从缓冲区中读取指定类型 T 的数据。
 * 它的主要功能是从缓冲区中提取一个 T 类型的值，
 * 并更新偏移量 mOffset，以便后续读取操作可以从正确的位置开始。
 */
  template <class T>
  T Get() {
    if (mOffset + sizeof(T) > mLength)
      throw "Buffer::Get(): Can't read any " + std::string(typeid(T).name()) + ".";
    T result = *((T *)(mpBuffer + mOffset));
    mOffset += sizeof(T);
    return result;
  }

  template <class T>
  T BackGet() {
    if (mBackOffset - sizeof(T) > mLength)
      throw "Buffer::BackGet(): Can't read  " + std::string(typeid(T).name()) + ".";
    T result = *((T *)(mpBuffer + mBackOffset - sizeof(T)));
    mBackOffset -= sizeof(T);
    return result;
  }

  template <class T>
  T *Get(size_t n) {
    if (Get<size_t>() == 0) return NULL;
    if (mOffset + sizeof(T) * n > mLength)
      throw "Buffer::Get(n): Can't read  " + std::string(typeid(T).name()) + ".";
    T *result = new T[n];
    memmove((char *)result, mpBuffer + mOffset, sizeof(T) * n);
    mOffset += sizeof(T) * n;
    return result;
  }

  template <class T>
  T *Delegate(size_t n = 1) {
    size_t size = sizeof(T) * n;
    Add(size);
    if ((mLength + size) >= mSize) {
      mSize = ((mLength + size) / mBlockSize + 1) * mBlockSize;
      if ((mpBuffer = (char *)realloc(mpBuffer, mSize)) == NULL)
        throw "Buffer::Delegate(n): Can't reallocate memory.";
    }
    T *dst = (T *)(mpBuffer + mLength);
    mLength += size;
    mBackOffset = mLength;
    return dst;
  }

  template <class T>
  T *Assign(size_t n = 1) {
    if (Get<size_t>() == 0) return NULL;

    if (mOffset + sizeof(T) * n > mLength) {
      throw "Buffer::Assign(n): Can't read  " + std::string(typeid(T).name()) + ".";
    }
    T *result = (T *)(mpBuffer + mOffset);
    mOffset += sizeof(T) * n;
    return result;
  }

  template <class T>
  T *AssignAll() {
      size_t size = Get<size_t>();
      if (size == 0) return NULL;
      size_t n = size / sizeof(T);
      if (mOffset + sizeof(T) * n > mLength)
          throw "Buffer::AssignAll(): Can't read  " + std::string(typeid(T).name()) + ".";
      T *result = (T *)(mpBuffer + mOffset);
      mOffset += sizeof(T) * n;
      return result;
  }

  char *AssignString() {
    size_t size = Get<size_t>();
    return Assign<char>(size);
  }

  template <class T>
  T *BackAssign(size_t n = 1) {
    if (mBackOffset - sizeof(T) * n > mLength)
      throw "Buffer::BackAssign(n): Can't read  " + std::string(typeid(T).name()) + ".";
    T *result = (T *)(mpBuffer + mBackOffset - sizeof(T) * n);
    mBackOffset -= sizeof(T) * n + sizeof(size_t);
    return result;
  }

  template <class T>
  T GetFromMarshal() {
    return (T)Get<gvirtus::common::pointer_t>();
  }

// 判断Buffer是否为空
  inline bool Empty() { return mOffset == mLength; }

  void Reset();
  void Reset(Communicator *c);
  /**
   * const char *：返回的指针指向的内容是常量。
   * char *const：返回的指针本身是常量。
   * GetBuffer() const：该成员函数是常量成员函数。它不会修改成员变量的值，所以它可以被const对象调用。
   */
  const char *const GetBuffer() const;
  size_t GetBufferSize() const;
  // 将Buffer的内容输出到Communicator
  void Dump(Communicator *c) const;

 private:
//  mBlockSize代表一个内存块
  size_t mBlockSize;
  // mSize代表当前buffer的总大小
  size_t mSize;
  // mLength代表当前已经使用的buffer大小
  size_t mLength;
  // 
  size_t mOffset;
  size_t mBackOffset;
  char *mpBuffer;
  bool mOwnBuffer;
};
}  // namespace gvirtus::communicators
