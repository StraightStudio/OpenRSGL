ARCH=`uname -m`
X64="x86_64"

cd data/
unzip data
cd ../

if [[ $ARCH = $X64 ]]; then
	echo "Running in 64-bit mode"
	LD_LIBRARY_PATH=bin/lib64 ./Stratwenty
else
	echo "Running in 32-bit mode"
	LD_LIBRARY_PATH=bin/lib32 ./Stratwenty.x86
fi

cd data/
rm -rf audio fonts img models res scenes
cd ../
