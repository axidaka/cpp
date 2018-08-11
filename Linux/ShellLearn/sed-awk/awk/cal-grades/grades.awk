# grades.awk --计算学生的平均成绩并确定字母等级以及全班平均成绩
# $1 = 学生姓名; $2- $NF = 测试成绩

# 输出字段的分隔符设置为制表符
BEGIN {OFS = "\t"}

# 对所有输入行所执行的操作
{
    #累计单人成绩
    total = 0
    for (i = 2; i <= NF; ++i)
        total += $i

    # 平均值
    avg = total / (NF - 1)
    # 保存平均值到数组, 下标值为行号
    student_avg[NR] = avg

    # 确定字母等级
    if (avg >= 90)  grade = "A"
    else if (arg >= 80) grade = "B"
    else if (arg >= 70) grade = "C"
    else if (arg >= 60) grade = "D"
    else  grade = "F"

    print "grade:", grade
    # 保存当前等级的人数
    ++ class_grade[grade]
    
    print "class_grade:", class_grade[grade]
    # 打印学生数据
    print $1, avg, grade
}

# 打印全班的统计情况
END{
    for (x = 1; x <= NR; ++x)
        class_avg_total += student_avg[x]

    class_average = class_avg_total / NR

    #确定多少人超过/低于平均分
    for (x = 1; x <= NR; ++x)
    {
        if (student_avg[x] >= class_average)
            ++ above_average
        else
            ++ below_average
     }

    #打印结果
    print ""
    print "Class Average:", class_average
    print "At or Above Average:", above_average
    print "Below Average:", below_average

    #打印每个字母等级的学生数
    for (letter_grade in class_grade)
        print letter_grade ":" class_grade[letter_grade] | "sort"

}

