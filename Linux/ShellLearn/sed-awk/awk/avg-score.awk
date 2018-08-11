#avg-score.awk 求五个成绩的平均值
{
    print "字段总数", NR,NF
    total = $2 + $3 + $4 + $5 + $6
    avg = total / 5
    print $1, avg
    }

END{
    print "done"
    }
