# blocklist.awk -- 用块格式打印姓名和地址
# 输入文件-名字,公司,街道,城市,州和邮编,电话
BEGIN { FS = ","   # comma-separated fields
}

{
    print "" #output blank line
    print $1 #name
    print $2 #company
    print $3 #street
    print $4, $5 #city, state zip
    print $6 #call
}

