#cmake_minimum_required(VERSION 3.25)
#project(dd_skyline CXX)
#
#set(CMAKE_CXX_STANDARD  11)
## 设置编译选项
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
#
## 添加源文件目录
#aux_source_directory(include SOURCE_FILES)
#aux_source_directory(thirdparty SOURCE_FILES)
#aux_source_directory(test SOURCE_FILES)
##定义两个变量，表示头文件路径和库路径
#find_package(OpenSSL REQUIRED)
## 显示的包含头文件
#include_directories(include)
#
#if(OPENSSL_FOUND)
#
#    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#
#    add_executable(drq_test ${SOURCE_FILES})
#
#    target_include_directories(${PROJECT_NAME} PUBLIC include)
#    target_include_directories(drq_test PUBLIC include)
#    # 链接 OpenSSL 库
#    target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto eTPSS)
#    target_link_libraries(drq_test OpenSSL::SSL OpenSSL::Crypto eTPSS)
#
#endif (OPENSSL_FOUND)

cmake_minimum_required(VERSION 3.25)
project(dd_skyline CXX)

set(CMAKE_CXX_STANDARD  11)
# 设置编译选项
# 强制使用静态链接的 OpenSSL 库
set(OPENSSL_INCLUDE_DIR "D:\\software\\lib\\OpenSSL\\include")
set(OPENSSL_CRYPTO_LIBRARY "D:\\software\\lib\\OpenSSL\\lib\\vc\\x64\\MT\\libcrypto_static.lib")
set(OPENSSL_SSL_LIBRARY "D:\\software\\lib\\OpenSSL\\lib\\vc\\x64\\MT\\libssl_static.lib")


# 添加源文件目录
aux_source_directory(include SOURCE_FILES)
aux_source_directory(test SOURCE_FILES)
aux_source_directory(thirdparty SOURCE_FILES)
# 显示的包含头文件
include_directories(include ${OPENSSL_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_executable(dd_skyline_test ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(dd_skyline_test PUBLIC include)
# 链接 OpenSSL 库
target_link_libraries(${PROJECT_NAME}  ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} ws2_32 Crypt32)
target_link_libraries(dd_skyline_test ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} ws2_32 Crypt32)