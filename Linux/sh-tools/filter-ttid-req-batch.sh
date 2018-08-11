#!/bin/sh

# 先通过查找syncall fail的ttid, 再zgrep该ttid的日志，打印日志行, 一直找到req请求行, 输出分隔符, 继续输出
# 用法

if [ $# -lt 3 ]
then
	echo "[usage] gzfilepath grepkey reqkey"
	exit 0
else
	gzfilepath=$1
	grepkey=$2
	reqkey=$3
	if [ -f $gzfilepath ]
	then
		echo "log.gz path: " $gzfilepath
		file $gzfilepath
	else
		echo "$gzfilepath not exist"
		exit 0
	fi
fi

filetype=${gzfilepath##*.}

if [ $filetype = "gz" ]
then
	
	zgrep -ie "$grepkey" $gzfilepath | awk '
	
	# 对筛选出特定时间+ttid的日志进行处理

	{
			print 
			if ($0 ~ /'"$reqkey"'/){

				print "\nfind XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
				print "\n"
			}
	}


	' | more

else

	grep -ie "$grepkey" $gzfilepath | awk '

	# 对筛选出特定时间+ttid的日志进行处理

	{
			print 
			if ($0 ~ /'"$reqkey"'/){

				print "\nfind XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
				print "\n"
			}
	}


	' | more

fi

