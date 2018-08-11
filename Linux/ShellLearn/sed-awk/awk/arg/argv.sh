#!/bin/sh
awk '
# argv.sh -打印命令行参数
BEGIN{
    for (i = 0; i < ARGC; ++i)
        print ARGV[i]
    print "Arg Count: " ARGC
}
' $*
