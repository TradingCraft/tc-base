#
# Base library unit tests
#

file(GLOB test_srcs CONFIGURE_DEPENDS *_t.cc)

add_executable(base_t ${test_srcs})

target_link_libraries(base_t
  PRIVATE
    base
    GTest::gtest
)

gtest_discover_tests(base_t)
