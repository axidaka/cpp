# ps-grep-kill.awk 
# INPUTFILE包含ps -ef | grep 的输出 通过参数传递进来awk
# PNAME 传递进来的进程名

BEGIN{
    FS=" "
    # 将ps -ef | grep 的输出文件加载到数组
    pidcount=0
    current_pid=0

    while (getline < INPUTFILE > 0){

        for ( i = 8 ; i<=NF; i++  ){
            pname=(pname" "$i)
         }

         #if (pname ~ /ps-grep-kill|grep/){
         #   print "catch ps-grep-kill|grep" pname
         #   pname=""
         #   continue
         #}
      
         if ($2 in array){
            
         }
         else{
            # array存储 数组下标为pid的pname
	        array[$2]=pname    
  	        # pid_array从下标0开始存储pid 
            pid_array[pidcount++]=$2
         }
        pname=""
     }
    
    # for (pid in array){
    #    print pid " " array[pid]
    # }
    # for (i = 0; i < pidcount; i ++){
    #    print pid_array[i]
    #}

	if (pidcount > 0){
		display_choice()
	}
	else{
		exit
	}
}

# 处理用户对kill 进程的响应
{
    if ($0 ~ /[Yy]|[Yy][Ee][Ss]/){
        kill_process()
    }

    current_pid++
    
    if (current_pid == pidcount){
        print "#end"
        exit
    }
    
    display_choice()
}

# 显示即将kill的进程pid pname 
function display_choice(){
    print "kill (pid: " pid_array[current_pid] " pname: " array[pid_array[current_pid]] " ) ?"
    print "y[es] or enter to escape"
 }

 function kill_process(){
    killcmd="sudo kill -s 9" " " pid_array[current_pid]
    print "run: " " " killcmd "\n"
    system(killcmd)
 }


