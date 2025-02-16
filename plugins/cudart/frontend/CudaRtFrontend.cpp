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


#include "CudaRtFrontend.h"

using namespace std;

using gvirtus::common::mappedPointer;
using gvirtus::common::pointer_t;


extern "C" {
/**
 * 初始化CUDA模块的函数。
 * 该函数目前为空，可能是一个占位符，用于将来实现CUDA模块的初始化逻辑。
 */
void __cudaInitModule() {}
}


/**
 * __attribute_used__ 是 GCC 编译器的一个属性，用于告诉编译器即使某个变量或函数没有被显式使用，也不要将其优化掉。
 *它的作用是确保变量或函数在编译后的二进制文件中保留，即使它没有被直接引用。
 *这在一些特殊的场景下非常有用，比如在某些库中，有一些函数是供外部调用的，
 *但是这些函数在库中并没有被直接调用，而是通过函数指针的方式传递给外部调用者，这时候就可以使用 __attribute_used__ 来确保这些函数不被优化掉。
 */
//全局变量，因为定义了无参构造函数，在动态链接库加载的时候会直接初始化。
// 该方法会触发CudaRtFrontend的构造函数，从而初始化全局变量。
CudaRtFrontend msInstance __attribute_used__;


/**
 * 在你的代码中，
 * 将静态变量初始化为NULL是为了确保
 * 在CudaRtFrontend类的构造函数中可以安全地检查这些变量是否已经被初始化，
 * 并在需要时进行动态分配。
 * 这样可以避免在构造函数中重复分配内存，
 * 从而提高代码的效率和可维护性。
 */
// CudaRtFrontend类的静态变量，初始化为null

/**
 * 定义一个静态成员变量 mappedPointers，用于存储映射指针的映射关系
 * 初始化为 NULL，表示尚未分配内存
 */
map<const void*, mappedPointer>* CudaRtFrontend::mappedPointers = NULL;

/**
 * 定义一个静态成员变量 devicePointers，用于存储设备指针的集合
 * 初始化为 NULL，表示尚未分配内存
 */
set<const void*>* CudaRtFrontend::devicePointers = NULL;

/**
 * 定义一个静态成员变量 toManage，用于存储线程与栈指针的映射关系
 * 初始化为 NULL，表示尚未分配内存
 */
map<pthread_t, stack<void*>*>* CudaRtFrontend::toManage = NULL;

/**
 * 定义一个静态成员变量 mapHost2DeviceFunc，用于存储主机到设备函数的映射关系
 * 初始化为 NULL，表示尚未分配内存
 */
map<const void*, std::string>* CudaRtFrontend::mapHost2DeviceFunc = NULL;

/**
 * 定义一个静态成员变量 mapDeviceFunc2InfoFunc，用于存储设备函数到信息函数的映射关系
 * 初始化为 NULL，表示尚未分配内存
 */
map<std::string, NvInfoFunction>* CudaRtFrontend::mapDeviceFunc2InfoFunc = NULL;

// 定义了构造函数
CudaRtFrontend::CudaRtFrontend() {
  if (devicePointers == NULL) devicePointers = new set<const void*>();
  if (mappedPointers == NULL) mappedPointers = new map<const void*, mappedPointer>();
  
  if (mapHost2DeviceFunc == NULL) mapHost2DeviceFunc = new map<const void*, std::string>();
  if (mapDeviceFunc2InfoFunc == NULL) mapDeviceFunc2InfoFunc = new map<std::string, NvInfoFunction>();

  if (toManage == NULL) toManage = new map<pthread_t, stack<void*>*>();
  gvirtus::frontend::Frontend::GetFrontend();
}

// static CudaRtFrontend* CudaRtFrontend::GetFrontend(){
//    return (CudaRtFrontend*) Frontend::GetFrontend();
//}
