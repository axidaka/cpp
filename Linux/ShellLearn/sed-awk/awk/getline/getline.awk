# getline.awk -- 测试getline 函数
BEGIN{
    printf "Enter your name: "
    getline < "-"
    print 
    printf "Enter your age: "
    getline age < "-"
    print age

    print "---------------------------------------------"
    printf "read file getline-test: "
    while ( (getline < "getline-test" ) > 0 )
        print

    print "---------------------------------------------"
    print "print 'cat /etc/passwd'"
    while ("cat /etc/passwd" | getline)
        print $0
        }
    

/^getline-flag/{
        while (getline) #读取下一行，继续执行脚本
            print $0 #相关系统变量都会被更新
    }
    
