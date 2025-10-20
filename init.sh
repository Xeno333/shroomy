sudo apt install libsdl2 libsdl2-mixer libsdl2-image libluajit-5.1-dev
mkdir libs/
git clone https://luajit.org/git/luajit.git libs/luajit/
cd libs/luajit
make -j$(nproc)
cd ../..