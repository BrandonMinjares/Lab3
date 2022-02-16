../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 -p build/tests/userprog/wait-killed -a wait-killed -p build/tests/userprog/child-bad -a child-bad -- -q  -f run wait-killed 
