#pragma once

#include <dlfcn.h>
#include <memory>
#include <string>
#include <iostream>

//#define ldDEBUG

namespace gvirtus::common {

    template<class T, typename... Args> class LD_Lib {
    private:
        using create_t = std::shared_ptr<T>(Args...);

        /* DLOPEN */
        /**
         * @brief 打开动态链接库
         * 
         * @param path 动态链接库的路径
         * @param flag 打开标志，默认为RTLD_LAZY
         * 
         * @throws std::string 如果加载失败，抛出错误信息
         */
        void dlopen(const std::string &path, int flag = RTLD_LAZY) {
#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::dlopen(path, flag) called" << std::endl;
#endif

            // 使用dlopen函数打开动态链接库
            m_module = ::dlopen(path.c_str(), flag);

            // 如果打开失败，抛出异常
            if (m_module == nullptr) {
                throw "Error loading: " + std::string(dlerror());
            }

#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::dlopen(path, flag) returned" << std::endl;
#endif
        }

        /* SET_CREATE_SYMBOL */
        /**
         * @brief 设置动态链接库中的创建函数符号
         * 
         * @param function 要设置的创建函数的名称
         * 
         * @throws std::string 如果加载符号失败，抛出错误信息
         */
        void set_create_symbol(const std::string &function) {
#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::set_create_symbol(&function) called" << std::endl;
            std::cout << "function: " << function.c_str() << std::endl;
            if (m_module == nullptr) {
                std::cout << "m_module is null!!! " << std::endl;
            }
#endif
            // 清除之前的错误状态
            dlerror();
            // 在动态链接库中查找指定的创建函数符号
            sym = (create_t *) dlsym(m_module, function.c_str());
            // 获取并保存当前的错误状态
            auto dl_error = dlerror();

            // 如果加载符号失败，抛出异常
            if (dl_error != nullptr) {
#ifdef ldDEBUG
                std::cout << "LD_Lib::set_create_symbol(&function) exception!" << std::endl;
#endif
                std::string error(dl_error);
                ::dlclose(m_module);
                throw "Cannot load symbol create: " + error;
            }

#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::set_create_symbol(&function) returned" << std::endl;
#endif
        }

    public:
        /* LD_LIB CONSTRUCTOR */
        /**
         * @brief LD_Lib类的构造函数
         * 
         * @param path 动态链接库的路径
         * @param fcreate_name 创建函数的名称，默认为"create_t"
         * 
         * @throws std::string 如果加载失败，抛出错误信息
         */
        LD_Lib(const std::string path, std::string fcreate_name = "create_t") {
#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::LD_Lib(path, fcreate_name) called (it's the constructor)" << std::endl;
            std::cout << "path: " << path << std::endl;
            std::cout << "fcreate_name: " << fcreate_name << std::endl;
#endif

            // 初始化对象指针为空
            _obj_ptr = nullptr;
            // 打开动态链接库
            dlopen(path);
            // 设置创建函数符号
            set_create_symbol(fcreate_name);

#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::LD_Lib(path, fcreate_name) returned" << std::endl;
#endif
        }

        /* LD_LIB DESTRUCTOR */
        /**
         * @brief LD_Lib类的析构函数
         * 
         * 该析构函数负责清理LD_Lib对象所占用的资源。
         * 具体来说，它会释放动态链接库的句柄，并重置对象指针。
         */
        ~LD_Lib() {
            // 检查动态链接库句柄是否为空
            if (m_module != nullptr) {
                // 重置创建函数指针
                sym = nullptr;
                // 重置对象指针
                _obj_ptr.reset();
                // 关闭动态链接库
                ::dlclose(m_module);
            }
        }

        /* BUILD_OBJ */
        /**
         * @brief 构建对象
         * 
         * 该函数用于创建一个类型为T的对象，并将其存储在LD_Lib类的成员变量_obj_ptr中。
         * 
         * @param args 可变参数列表，用于传递给创建函数的参数。
         * 
         * @throws std::string 如果创建对象失败，抛出错误信息。
         */
        void build_obj(Args... args) {
#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::build_obj(Args... args) called" << std::endl;
#endif

            // 调用创建函数，创建对象并将其存储在_obj_ptr中
            _obj_ptr = this->sym(args...);

#ifdef ldDEBUG
            // 输出调试信息
            std::cout << "LD_Lib::build_obj(Args... args) returned" << std::endl;
#endif
        }

        /* OBJ_PTR */
        std::shared_ptr<T> obj_ptr() {
            return _obj_ptr;
        }

    protected:
        create_t *sym;
        void *m_module;
        std::shared_ptr<T> _obj_ptr;
    };

} // namespace gvirtus::common
