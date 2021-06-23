# author 	kiraskyler
# data		20150526
# connect 	kiraskyler@163.com
# version 	V0.0.3.20210526
# describe  create .gitignore in every dir, create .gitkeep in every emputy dir

# how to use
# ./git.sh or ./git.sh ./		input path or select local path to do

#################### environment ####################
#!/bin/bash

#################### code ###########################
file_path=$(readlink -f "$(dirname "$0")")
work_path=`pwd`
work=true
force=false
log=false
remove=false

func_log()
{
	if [ $log == true ]
	then
		echo " --- $1 --- "
	fi 
}

dir_is_emputy()
{
	if [ $force == true ] && [ -f $1/.gitkeep ]
	then
		return 0
	else
		return `ls -A $1 | wc -w`
	fi
}

get_dir()
{
	if [ -d $1 ]
	then 
		work_path=$1
	else
		echo "input dir path erro!"
		exit -1
	fi
}

echo_print_newline()
{
for ((i=0;i<$1;i++))
do
	echo -n -e "\t"
done
}

func_dir_remove()
{
	if [ ! -f $1/$2/git_keep_ignore.sh ]
	then
		rm -f $1/$2/.gitignore $1/$2/.gitkeep 2> /dev/null
		echo_print_newline $3
		echo -e "[r] $2 "
	fi
}

func_dir_touch()
{
	# func_log " func_dir_touch $1/$2/ "
	if dir_is_emputy $1/$2 == 0
	then
		cp -f $file_path/.gitkeep  $1/$2/.gitkeep 2> /dev/null || true			# 空目录放
		func_log " cp -f $file_path/.gitkeep  $1/$2/.gitkeep 2> /dev/null || true "
		echo_print_newline $3
		echo -e "[v] $2 "
	else
		echo_print_newline $3
		echo -e "[x] $2 "
	fi
	cp -f $file_path/.gitignore $1/$2/.gitignore 2> /dev/null || true		# 所有目录都放该文件
}


func_dir_loop()
{
	func_log " loop $1 "
	# func_dir_touch_file $1 "/" $2

	for path in `ls $1`
	do
		if [ -d $1/$path ] 				# path is dir
		then
			if [ $remove == false ]
			then
				func_dir_touch $1 $path $2
			else
				func_dir_remove $1 $path $2
			fi
			
			func_dir_loop $1/$path $2+1
		fi
	done
}

while getopts "d:fhlr" arg
do
	case $arg in
		d)
			get_dir $OPTARG
			;;
		f)
			force=true
			;;
		l)
			log=true
			;;
		h)
			echo -e "normal: ./git_keep_ignore.sh -d ./"
			echo -e "normal remove: ./git_keep_ignore.sh -d ./ -r"
			echo
			echo -e "\t-d dir [path], work in place path, if not place -d, it work in ./"
			echo -e "\t-f force, work force, maybe replace .gitkeep and .gitignore"
			echo -e "\t-h help, now you see that"
			echo -e "\t-l log, echo log"
			echo -e "\t-r remove, remove .gitkeep and .gitignore"
			work=false
			;;
		r)
			remove=true
			;;
		\?)
			echo "???"
			;;
		esac
done

# loop dirs touch file
# func_log " $file_path "

if [ $work == true ]
then
	func_dir_loop $work_path 0
fi

# end