# GP_Task

## Build

Use cmake to build the project. 
```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## Run

To run code execute `./GP_Task`.
To exit press `Enter`.

## messageQueue
Mutexes are added to the message queue for thread safety in a hypothetical expansion of the code where multiple threads would access the read/write methods. For this particular case, they are not needed.

## Memory leaks check 

Valgrind output

```
==24862== HEAP SUMMARY:
==24862==     in use at exit: 0 bytes in 0 blocks
==24862==   total heap usage: 12 allocs, 12 frees, 75,729 bytes allocated
==24862== 
==24862== All heap blocks were freed -- no leaks are possible
==24862== 
==24862== For lists of detected and suppressed errors, rerun with: -s
==24862== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```