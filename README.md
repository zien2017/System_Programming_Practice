# malloc_r
| **Thread-safe  Dynamic Memory Allocation Realization** **(Clang)** |      |
| ------------------------------------------------------------ | ---- |
| -  Implemented a malloc  library with first fit and best fit using system call brk() and sbrk();   <br />-  Organize memory space  with Bidirectional Coalescing, an implicit list, raised by Prof. Donald  Knuth;  <br />-  Optimize first fit and  best fit by maintaining an explicit linked list;   <br />-  Realized read-lock  and write-lock using Semaphore, to ensure thread safety.  <br />-  *https://github.com/branwn/malloc_r* |      |

To avoid cheating, the repository will be published after the grading.
