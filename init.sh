mkdir libs/
git clone https://luajit.org/git/luajit.git libs/luajit/
cd libs/luajit
ls
make -j$(nproc)
cd ../..