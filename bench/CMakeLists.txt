ADD_EXECUTABLE (bench
    bench.cpp
)

TARGET_LINK_LIBRARIES (bench
    datastore-cpp-lib
    benchmark
)
