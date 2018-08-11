#checkbook.awk 计算账号消费余额

BEGIN{
    FS = ";"
    }


    #在行号为1时, 获取总余额
    NR == 1 {
        print "Beginning Balance:\t" $1
        balance = $1
        next   #取得下一条记录并结束
        }

    {
        print $1, $2,$3
        balance += $3
        print balance
    }
