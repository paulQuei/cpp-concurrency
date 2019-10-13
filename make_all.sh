rm -rf bin

mkdir build
cd build

cmake ../src/
make

#cmake ../mm/
#make

cd ../
mv build/bin ./
rm -rf build/
