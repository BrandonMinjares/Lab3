../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 -p build/tests/userprog/read-bad-ptr -a read-bad-ptr -p ../tests/userprog/sample.txt -a sample.txt -- -q  -f run read-bad-ptr 
