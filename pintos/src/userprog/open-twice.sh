../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 -p build/tests/userprog/open-twice -a open-twice -p ../tests/userprog/sample.txt -a sample.txt -- -q  -f run open-twice 
