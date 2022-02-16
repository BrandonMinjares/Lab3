../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 -p build/tests/userprog/write-bad-ptr -a write-bad-ptr -p ../tests/userprog/sample.txt -a sample.txt -- -q  -f run write-bad-ptr 
