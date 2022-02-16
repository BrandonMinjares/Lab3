/* 
 * This file is derived from source code for the Pintos
 * instructional operating system which is itself derived
 * from the Nachos instructional operating system. The 
 * Nachos copyright notice is reproduced in full below. 
 *
 * Copyright (C) 1992-1996 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose, without fee, and
 * without written agreement is hereby granted, provided that the
 * above copyright notice and the following two paragraphs appear
 * in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
 * AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
 * BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 *
 * Modifications Copyright (C) 2017-2021 David C. Harrison.  
 * All rights reserved.
 */

#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userprog/tss.h"
#include "userprog/elf.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "vm/frame.h"
#include "vm/page.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/syscall.h"
#include "userprog/process.h"
#include "devices/timer.h"

/*
 * Push the command and arguments found in CMDLINE onto the stack, word 
 * aligned with the stack pointer ESP. Should only be called after the ELF 
 * format binary has been loaded by elf_load();
 */
static void
push_command(const char *cmdline UNUSED, void **esp)
{
  printf("Base Address: 0x%08x\n", (unsigned int)*esp);

  // Word align with the stack pointer.
  *esp = (void *)((unsigned int)(*esp) & 0xfffffffc);

  // Some of your CSE130 Lab 3 code will go here.
  //
  // You'll be doing address arithmetic here and that's one of only a handful
  // of situations in which it is acceptable to have comments inside functions.
  //
  // As you advance the stack pointer by adding fixed and variable offsets
  // to it, add a SINGLE LINE comment to each logical block, a comment that
  // describes what you're doing, and why.
  //
  // If nothing else, it'll remind you what you did when it doesn't work :)
}

/* 
 * A thread function to load a user process and start it running. 
 * CMDLINE is assumed to contain an executable file name with no arguments.
 * If arguments are passed in CMDLINE, the thread will exit imediately.
 */
static void
start_process(void *cmdline)
{
  // Initialize interrupt frame and load executable.
  struct intr_frame pif;
  memset(&pif, 0, sizeof pif);

  pif.gs = pif.fs = pif.es = pif.ds = pif.ss = SEL_UDSEG;
  pif.cs = SEL_UCSEG;
  pif.eflags = FLAG_IF | FLAG_MBS;

  bool loaded = elf_load(cmdline, &pif.eip, &pif.esp);
  if (loaded)
    push_command(cmdline, &pif.esp);
 
  palloc_free_page(cmdline);

  if (!loaded)
    thread_exit();

  // Start the user process by simulating a return from an
  // interrupt, implemented by intr_exit (in threads/intr-stubs.S).
  // Because intr_exit takes all of its arguments on the stack in
  // the form of a `struct intr_frame',  we just point the stack
  // pointer (%esp) to our stack frame and jump to it.
  asm volatile("movl %0, %%esp; jmp intr_exit" : : "g"(&pif) : "memory");
  NOT_REACHED();
}

/*  
 * Starts a new kernel thread running a user program loaded from CMDLINE. 
 * The new thread may be scheduled (and may even exit) before process_execute() 
 * returns.  Returns the new process's thread id, or TID_ERROR if the thread 
 * could not be created. 
 */
tid_t 
process_execute(const char *cmdline)
{
  // Make a copy of CMDLINE to avoid a race condition between the caller 
  // and elf_load()
  char *cmdline_copy = palloc_get_page(0);
  if (cmdline_copy == NULL)
    return TID_ERROR;

  strlcpy(cmdline_copy, cmdline, PGSIZE);

  // Create a Kernel Thread for the new process
  tid_t tid = thread_create(cmdline, PRI_DEFAULT, start_process, cmdline_copy);

  // CSE130 Lab 3 : The "parent" thread immediately returns after creating
  // the child. To get ANY of the tests passing, you need to synchronise the
  // activity of the parent and child threads.

  return tid;
}

/* 
 * Waits for thread TID to die and returns its exit status.  If it was 
 * terminated by the kernel (i.e. killed due to an exception), returns -1.
 * If TID is invalid or if it was not a child of the calling process, or 
 * if process_wait() has already been successfully called for the given TID, 
 * returns -1 immediately, without waiting.
 *
 * This function will be implemented in CSE130 Lab 3. For now, it does nothing. 
 */
int 
process_wait(tid_t child_tid UNUSED)
{
  return -1;
}

/* Free the current process's resources. */
void 
process_exit(void)
{
  struct thread *cur = thread_current();
  uint32_t *pd;

  // Destroy the current process's page directory and switch back
  // to the kernel-only page directory. 
  pd = cur->pagedir;
  if (pd != NULL)
  {
    // Correct ordering here is crucial.  We must set
    // cur->pagedir to NULL before switching page directories,
    // so that a timer interrupt can't switch back to the
    // process page directory.  We must activate the base page
    // directory before destroying the process's page
    // directory, or our active page directory will be one
    // that's been freed (and cleared). 
    cur->pagedir = NULL;
    pagedir_activate(NULL);
    pagedir_destroy(pd);
  }
}

/* 
 * Sets up the CPU for running user code in the current thread.
 * This function is called on every context switch. 
 */
void 
process_activate(void)
{
  struct thread *t = thread_current();

  // Activate thread's page tables. 
  pagedir_activate(t->pagedir);

  // Set thread's kernel stack for use in processing interrupts. 
  tss_update();
}
