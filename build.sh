THREADS=$(expr "`nproc`*2" | bc)
echo "Using $THREADS threads"
make -j$THREADS all
