# argv.awk - 打印命令行参数，不包括脚本本身和任何调用awk指定的选项
BEGIN{
    for (i  = 0; i < ARGC; ++i)
        print ARGV[i]
    print "Arg Count: " ARGC
}
