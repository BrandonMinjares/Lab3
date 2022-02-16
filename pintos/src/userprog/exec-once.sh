../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 -p build/tests/userprog/exec-once -a exec-once -p build/tests/userprog/child-simple -a child-simple -- -q  -f run exec-once 
