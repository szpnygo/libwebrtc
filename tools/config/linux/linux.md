cmake -G Ninja -DCMAKE_CXX_FLAGS="-stdlib=libc++" ..
ninja -t clean
ninja example