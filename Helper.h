#pragma once

#ifdef DEBUG
#define LOG(...) \
    do { \
        std::cout << "[DEBUG] " << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ") - "; \
        ::log_helper(__VA_ARGS__); \
        std::cout << std::endl; \
    } while(0)

// 辅助函数模板：支持任意数量、任意类型的参数（需支持 operator<<）
template<typename... Args>
void log_helper(const Args&... args) {
    (std::cout << ... << args);  // C++17 折叠表达式
}
#endif
