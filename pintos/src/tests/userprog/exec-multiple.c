/* Executes and waits for multiple child processes. */

#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void) 
{
  int p1 = exec ("child-simple");
  int p2 = exec ("child-simple");
  wait(p1);
  int p3 = exec ("child-simple");
  wait(p2);
  wait(p3);
  int p4 = exec ("child-simple");
  wait(p4);
}
