#cmake_minimum_required(VERSION 3.25)
#project(dd_skyline CXX)
#
#set(CMAKE_CXX_STANDARD  11)
## 设置编译选项
## 强制使用静态链接的 OpenSSL 库
#set(OPENSSL_INCLUDE_DIR "D:\\software\\lib\\OpenSSL\\include")
#set(OPENSSL_CRYPTO_LIBRARY "D:\\software\\lib\\OpenSSL\\lib\\vc\\x64\\MT\\libcrypto_static.lib")
#set(OPENSSL_SSL_LIBRARY "D:\\software\\lib\\OpenSSL\\lib\\vc\\x64\\MT\\libssl_static.lib")
#aux_source_directory(thirdparty SOURCE_FILES)
#aux_source_directory(include SOURCE_FILES)
#include_directories(include ${OPENSSL_INCLUDE_DIR})
#
#
#
## 编译为动态链接库（DLL）
#add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
#set_target_properties(${PROJECT_NAME} PROPERTIES
#        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
## 链接 OpenSSL 和 eTPSS 库
#target_link_libraries(${PROJECT_NAME}  ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} ws2_32 Crypt32)

#cmake_minimum_required(VERSION 3.25)
#project(dd_skyline CXX)
#
#set(CMAKE_CXX_STANDARD  11)
## 设置编译选项
## 强制使用静态链接的 OpenSSL 库
#set(OPENSSL_INCLUDE_DIR "D:\\software\\lib\\OpenSSL\\include")
#set(OPENSSL_CRYPTO_LIBRARY "D:\\software\\lib\\OpenSSL\\lib\\vc\\x64\\MT\\libcrypto_static.lib")
#set(OPENSSL_SSL_LIBRARY "D:\\software\\lib\\OpenSSL\\lib\\vc\\x64\\MT\\libssl_static.lib")
#
#
## 添加源文件目录
#aux_source_directory(include SOURCE_FILES)
#aux_source_directory(test SOURCE_FILES)
#aux_source_directory(thirdparty SOURCE_FILES)
## 显示的包含头文件
#include_directories(include ${OPENSSL_INCLUDE_DIR})
#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#add_executable(dd_skyline_test ${SOURCE_FILES})
#target_include_directories(${PROJECT_NAME} PUBLIC include)
#target_include_directories(dd_skyline_test PUBLIC include)
## 链接 OpenSSL 库
#target_link_libraries(${PROJECT_NAME}  ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} ws2_32 Crypt32)
#target_link_libraries(dd_skyline_test ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} ws2_32 Crypt32)

#cmake_minimum_required(VERSION 3.25)
#project(dd_skyline CXX)
#
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall")
##定义两个变量，表示头文件路径和库路径
#find_package(OpenSSL REQUIRED)
#
## 添加源文件目录
#aux_source_directory(include SOURCE_FILES)
#aux_source_directory(test SOURCE_FILES)
## 显示的包含头文件
#include_directories(include)
#
#if(OPENSSL_FOUND)
#    add_executable(${PROJECT_NAME} ${SOURCE_FILES} include)
#
#    add_executable(dd_skyline_test ${SOURCE_FILES} include)
#
#    target_include_directories(${PROJECT_NAME} PUBLIC include)
#    target_include_directories(SKQ_test PUBLIC include)
#    # 链接 OpenSSL 库
#    target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)
#    target_link_libraries(SKQ_test OpenSSL::SSL OpenSSL::Crypto)
#
#endif (OPENSSL_FOUND)

#cmake_minimum_required(VERSION 3.25)
#project(dd_skyline CXX)
#
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall")
##定义两个变量，表示头文件路径和库路径
#find_package(OpenSSL REQUIRED)
#
## 添加源文件目录
#aux_source_directory(include SOURCE_FILES)
#aux_source_directory(test SOURCE_FILES)
#aux_source_directory(thirdparty SOURCE_FILES)
## 显示的包含头文件
#include_directories(include)
#
#if(OPENSSL_FOUND)
#    add_executable(${PROJECT_NAME} ${SOURCE_FILES} )
#
#    add_executable(dd_skyline_test ${SOURCE_FILES} )
#
#    target_include_directories(${PROJECT_NAME} PUBLIC include)
#    target_include_directories(dd_skyline_test PUBLIC include)
#    # 链接 OpenSSL 库
#    target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)
#    target_link_libraries(dd_skyline_test OpenSSL::SSL OpenSSL::Crypto)
#
#endif (OPENSSL_FOUND)


# 下面是x86 linux上面的编译的文件，测试版本
cmake_minimum_required(VERSION 3.21)
project(dd_skyline CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# 设置 OpenSSL 库的路径
# 请将以下路径替换为你实际的 OpenSSL 静态库路径
set(OPENSSL_ROOT_DIR "/usr/local/openssl")
set(OPENSSL_LIBRARIES "/usr/local/openssl/lib/libssl.a;/usr/local/openssl/lib/libcrypto.a")

# 查找 OpenSSL 头文件
include_directories(include ${OPENSSL_ROOT_DIR}/include)

# 查找源文件
aux_source_directory(include SOURCE_FILES)
aux_source_directory(thirdparty SOURCE_FILES)

# 编译为动态链接库（Shared Library）
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 静态链接 OpenSSL 库
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})