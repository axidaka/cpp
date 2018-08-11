#!/bin/sh

# 将172.25.54.25的PC下共享目录common-component挂载到Linux /mnt/wind，方便linux访问
mount -t cifs -o username=903503127@qq.com,password=weiruan20120701 //172.25.54.25/common-component /mnt/wind

echo "ls -al /mnt/wind"
ls -al /mnt/wind
echo "\n cd /mnt/wind ?[y/n]"

read choice
case "$choice" in
	[Yy] | [Yy][Ee][Ss]) cd /mnt/wind ;;
	*) 		     echo "done"
esac



