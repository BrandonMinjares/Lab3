../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 -p build/tests/userprog/read-zero -a read-zero -p ../tests/userprog/sample.txt -a sample.txt -- -q  -f run read-zero 
