../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 -p build/tests/userprog/wait-simple -a wait-simple -p build/tests/userprog/child-simple -a child-simple -- -q  -f run wait-simple 
