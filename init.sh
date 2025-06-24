mkdir libs/
git clone https://luajit.org/git/luajit.git libs/luajit/
cd libs/luajit
make -j$(nproc)
cd ../..