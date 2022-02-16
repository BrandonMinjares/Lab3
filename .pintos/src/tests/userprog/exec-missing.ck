# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(exec-missing) begin
load: 'no-such-file': open failed, no such file
(exec-missing) exec("no-such-file"): -1
(exec-missing) end
exec-missing: exit(0)
EOF
pass;
