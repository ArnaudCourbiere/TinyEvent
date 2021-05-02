# find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
# include_directories (${TEST_SOURCE_DIR}/src
#                      ${Boost_INCLUDE_DIRS}
#                      )
# add_definitions (-DBOOST_TEST_DYN_LINK)
# add_executable (Test test.cpp)
# target_link_libraries (Test
#                        Sqr
#                        ${Boost_FILESYSTEM_LIBRARY}
#                        ${Boost_SYSTEM_LIBRARY}
#                        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
#                        )
# add_test (NAME MyTest COMMAND Test)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable (unittests tests.cpp)
target_link_libraries (unittests LINK_PUBLIC TinyEvent gtest_main)

include(GoogleTest)
gtest_discover_tests(unittests)
