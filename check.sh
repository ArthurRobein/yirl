#!/bin/bash
cppcheck -I include/core/ --enable=warning --enable=style --enable=performance --enable=portability  */*.c  */*/*.c 

rats */*.c  */*/*.c 
