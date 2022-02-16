# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(wait-twice) begin
(child-simple) run
child-simple: exit(1293)
(wait-twice) wait(exec()) = 1293
(wait-twice) wait(exec()) = -1
(wait-twice) end
wait-twice: exit(0)
EOF
pass;
