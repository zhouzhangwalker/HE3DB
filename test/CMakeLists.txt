add_executable(comparison_test comparison_test.cpp)
target_link_libraries(comparison_test HEDB)

add_executable(conversion_test conversion_test.cpp)
target_link_libraries(conversion_test HEDB)

add_executable(tpch_q6 tpch_q6.cpp)
target_link_libraries(tpch_q6 HEDB)
