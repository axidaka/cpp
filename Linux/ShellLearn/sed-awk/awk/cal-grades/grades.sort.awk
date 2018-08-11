# grade.sort.awk -- 对学生成绩进行排序的脚本
# 输入 grade.test

#排序函数: 采用简单插入排序,按升序
function insert_sort(ARRAY, ELEMENTS, temp, i, j)
{
    # 数组下标从1开始, 排序从2开始
    for (i = 2; i <= ELEMENTS; ++ i)
    {
        for (j = i; (j- 1 ) in ARRAY && ARRAY[j] < ARRAY[j - 1]; -- j)
        {
            #交换
            temp = ARRAY[j - 1]
            ARRAY[j -1 ] = ARRAY[j]
            ARRAY[j] =   temp
        }
    }
 }

# 主例程
{
    # 将当前行的学生成绩信息保存到数组grades
    for (i = 2; i <= NF; ++i)
        grades[i- 1] = $i # 成绩从2开始,1是姓名

    # 调用排序函数对成绩升序排序
    insert_sort(grades, NF -1)

    printf("%s:", $1)

    for (j = 1; j <= NF - 1; ++ j)
        printf("%d ", grades[j])
    printf("\n")
}
