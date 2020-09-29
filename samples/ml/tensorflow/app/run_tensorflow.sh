cp alpine-tensorflow-copy.img alpine-tensorflow.img
echo "Copied tensorflow image" 
sync
sleep 5
sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches'
echo "Caches dropped. Starting application... "
# TODO: Complete sgx-lkl parameters on the last line (not sure about this)
