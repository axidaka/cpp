# insert.awk -- 演示插入字符函数insert
function insert(STRING, POS, INS, before_tmp)
{
    before_tmp = substr(STRING, 1, POS)
    # after_tmp 不是该函数形参， 所以可以被主例程访问
    after_tmp = substr(STRING, POS + 1)
    return before_tmp INS after_tmp
}

# 主例程
{
    print "Function returns ", insert($1, 4, "Zhqs")
    print "The value of $1 after is ", $1
    print "The value of STRING is ", STRING
    print "The value of before_tmp is ", before_tmp
    print "The value of after_tmp is ", after_tmp
}
