# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(exec-multiple) begin
(child-simple) run
child-simple: exit(1181)
(child-simple) run
child-simple: exit(1181)
(child-simple) run
child-simple: exit(1181)
(child-simple) run
child-simple: exit(1181)
(exec-multiple) end
exec-multiple: exit(0)
EOF
pass;
