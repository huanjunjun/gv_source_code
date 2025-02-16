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
 * @file   Frontend.cpp
 * @author Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>
 * @date   Wed Sep 30 12:57:11 2009
 *
 * @brief
 *
 *
 */

#include <gvirtus/communicators/CommunicatorFactory.h>
#include <gvirtus/communicators/EndpointFactory.h>
#include <gvirtus/frontend/Frontend.h>

#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include <chrono>

#include <stdlib.h> /* getenv */
#include "log4cplus/configurator.h"
#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"

using namespace std;

using gvirtus::communicators::Buffer;
using gvirtus::communicators::Communicator;
using gvirtus::communicators::CommunicatorFactory;
using gvirtus::communicators::EndpointFactory;
using gvirtus::frontend::Frontend;

using std::chrono::steady_clock;

static Frontend msFrontend;
map<pthread_t, Frontend *> *Frontend::mpFrontends = NULL;
static bool initialized = false;

log4cplus::Logger logger;

std::string getEnvVar(std::string const &key) {
    char *env_var = getenv(key.c_str());
    return (env_var == nullptr) ? std::string("") : std::string(env_var);
}

/**
 * 初始化前端对象。
 * 该函数负责配置日志记录器，设置日志级别，获取配置文件路径，创建通信器并连接到后端。
 * 
 * @param c 指向通信器对象的指针。
 */
void Frontend::Init(Communicator *c) {
    
    // 配置基本的日志记录器
    log4cplus::BasicConfigurator basicConfigurator;
    basicConfigurator.configure();
    
    // 获取日志记录器实例
    // 这个宏函数是为了做编码适配的，来区分unicode编码与别的编码
    logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("GVirtuS Frontend"));

    // 获取日志级别环境变量，如果未设置则默认为 INFO_LOG_LEVEL
    std::string logLevelString = getEnvVar("GVIRTUS_LOGLEVEL");
    log4cplus::LogLevel logLevel = logLevelString.empty() ? log4cplus::INFO_LOG_LEVEL : std::stoi(logLevelString);

    // 设置日志记录器的日志级别
    logger.setLogLevel(logLevel);

    // 获取当前线程的ID
    pid_t tid = syscall(SYS_gettid);

    // 获取配置文件路径
    std::string config_path = getEnvVar("GVIRTUS_CONFIG");

    // 如果配置文件路径为空，则尝试从GVIRTUS_HOME环境变量中获取
    if (config_path.empty()) {
        config_path = getEnvVar("GVIRTUS_HOME") + "/etc/properties.json";
        
        // 如果GVIRTUS_HOME环境变量也为空，则使用默认路径
        if (config_path.empty()) {
            config_path = "./properties.json";
        }
    }

    // 声明一个默认的端点
    // 这是一个智能指针，用于自动管理内存
    // ！！！！！！！！！！！！！！！！！！！！！似乎并未使用
    // TODO: 这里的default_endpoint似乎没有被使用，应该删除
    std::unique_ptr<char> default_endpoint;

    // 如果当前线程的前端对象不存在，则创建一个新的前端对象
    if (mpFrontends->find(tid) == mpFrontends->end()) {
        Frontend *f = new Frontend();
        mpFrontends->insert(make_pair(tid, f));
    }

    // 记录前端版本信息
    LOG4CPLUS_INFO(logger, "🛈  - GVirtuS frontend version " + config_path);

    try {
        // 根据配置文件路径创建端点
        auto endpoint = EndpointFactory::get_endpoint(config_path);

        // 根据端点创建通信器并连接到后端
        mpFrontends->find(tid)->second->_communicator = CommunicatorFactory::get_communicator(endpoint);
        mpFrontends->find(tid)->second->_communicator->obj_ptr()->Connect();
    }
    catch (const string & ex) {
        // 记录错误信息并退出程序
        LOG4CPLUS_ERROR(logger, "✖ - " << fs::path(__FILE__).filename() << ":" << __LINE__ << ":" << " Exception occurred: " << ex);
        exit(EXIT_FAILURE);
    }

    // 初始化输入、输出和启动缓冲区
    mpFrontends->find(tid)->second->mpInputBuffer = std::make_shared<Buffer>();
    mpFrontends->find(tid)->second->mpOutputBuffer = std::make_shared<Buffer>();
    mpFrontends->find(tid)->second->mpLaunchBuffer = std::make_shared<Buffer>();
    
    // 设置退出码和初始化标志
    mpFrontends->find(tid)->second->mExitCode = -1;
    mpFrontends->find(tid)->second->mpInitialized = true;

}

Frontend::~Frontend() {
    if (mpFrontends == nullptr) {
        delete mpFrontends;
        return;
    }

    pid_t tid = syscall(SYS_gettid);  // getting frontend's tid

    auto env = getenv("GVIRTUS_DUMP_STATS");
    auto dump_stats =
            env != nullptr && (strcasecmp(env, "on") == 0 || strcasecmp(env, "true") == 0 || strcmp(env, "1") == 0);

    map<pthread_t, Frontend *>::iterator it;
    for (it = mpFrontends->begin(); it != mpFrontends->end(); it++) {
        if (dump_stats) {
            std::cerr << "[GVIRTUS_STATS] Executed " << it->second->mRoutinesExecuted << " routine(s) in "
                      << it->second->mRoutineExecutionTime << " second(s)\n"
                      << "[GVIRTUS_STATS] Sent " << it->second->mDataSent / (1024 * 1024.0) << " Mb(s) in "
                      << it->second->mSendingTime
                      << " second(s)\n"
                      << "[GVIRTUS_STATS] Received " << it->second->mDataReceived / (1024 * 1024.0) << " Mb(s) in "
                      << it->second->mReceivingTime
                      << " second(s)\n";
        }
        mpFrontends->erase(it);
    }
}

Frontend *Frontend::GetFrontend(Communicator *c) {
    // mpFrontends是Frontend类的一个静态变量
    // 由于为每个线程都创建了一个kv，所以不存在并发问题
    // Frontend不是单例，每个线程拥有一个
    if (mpFrontends == nullptr)
        mpFrontends = new map<pthread_t, Frontend *>();

    //获取当前thread id，用于查找Frontend
    pid_t tid = syscall(SYS_gettid);  // getting frontend's tid

    // 根据id查找 对应的Frontend* 实例，如果找到则返回
    if (mpFrontends->find(tid) != mpFrontends->end())
        return mpFrontends->find(tid)->second;

    // 找不到当前线程的Frontend则创建一个
    Frontend *f = new Frontend();
    try {
        f->Init(c);
        mpFrontends->insert(make_pair(tid, f));
    }
    catch (const char *e) {
        cerr << "Error: cannot create Frontend ('" << e << "')" << endl;
    }

    return f;
}

void Frontend::Execute(const char *routine, const Buffer *input_buffer) {
    if (input_buffer == nullptr) input_buffer = mpInputBuffer.get();

    pid_t tid = syscall(SYS_gettid);
    if (mpFrontends->find(tid) == mpFrontends->end()) {
        // error
        cerr << " ERROR - can't send any job request " << endl;//
        return;
    }

    /* sending job */
    auto frontend = mpFrontends->find(tid)->second;//获取当前线程的Frontend实例
    frontend->mRoutinesExecuted++;//记录执行的routine数量
    auto start = steady_clock::now();//记录开始时间
    frontend->_communicator->obj_ptr()->Write(routine, strlen(routine) + 1);//发送routine名称
    frontend->mDataSent += input_buffer->GetBufferSize(); //记录发送的数据量
    input_buffer->Dump(frontend->_communicator->obj_ptr().get()); //发送input_buffer
    frontend->_communicator->obj_ptr()->Sync();//同步
    frontend->mSendingTime += std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start) .count() / 1000.0;
    frontend->mpOutputBuffer->Reset();

    frontend->_communicator->obj_ptr()->Read((char *) &frontend->mExitCode, sizeof(int));
    double time_taken;
    frontend->_communicator->obj_ptr()->Read(reinterpret_cast<char *>(&time_taken), sizeof(time_taken));
    frontend->mRoutineExecutionTime += time_taken;

    start = steady_clock::now();
    size_t out_buffer_size;
    frontend->_communicator->obj_ptr()->Read((char *) &out_buffer_size, sizeof(size_t));
    frontend->mDataReceived += out_buffer_size;
    if (out_buffer_size > 0)
        frontend->mpOutputBuffer->Read<char>( frontend->_communicator->obj_ptr().get(), out_buffer_size);
    frontend->mReceivingTime += std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start).count() / 1000.0;
}

void Frontend::Prepare() {
    pid_t tid = syscall(SYS_gettid);
    if (this->mpFrontends->find(tid) != mpFrontends->end())
        mpFrontends->find(tid)->second->mpInputBuffer->Reset();
}
