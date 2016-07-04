all: test_colpack

test_colpack : test_colpack.cpp
	/usr/local/bin/g++ --std=c++11 -o $@ $^ -I${HOME}/packages/ColPack/include -L${HOME}/packages/ColPack/lib -lColPack
