#!/bin/bash

TIMEOUT=300 # 5 min
SO2_WORKSPACE=/linux/tools/labs
ASSIGNMENT0_MOD=list.ko
ASSIGNMENT0_DIR=${SO2_WORKSPACE}/skels/assignments/0-list
ASSIGNMENT0_CHECKER_DIR=${SO2_WORKSPACE}/skels/assignments/0-list-checker
ASSIGNMENT0_OUTPUT=${SO2_WORKSPACE}/skels/0-list-output
ASSIGNMENT0_FINISHED=${SO2_WORKSPACE}/skels/0-list-finished

usage()
{
	echo "Usage: $0 <assignment>"
	exit 1
}

timeout_exceeded()
{
	echo TIMEOUT EXCEEDED !!! killing the process
	pkill -SIGKILL qemu
	exit 1
}

run_checker()
{
	local assignment_mod=$1
	local assignment_dir=$2
	local checker_dir=$3
	local output=$4
	local finished=$5
	local assignment=$6
	
	local module_path="${assignment_dir}/${assignment_mod}"

	echo "Copying the contents of src/ into $assignment_dir"
	cp src/* $assignment_dir

	echo "Checking if $assignment_mod exists before build"
	if [ -f $module_path ]; then
			echo "$assignment_mod shouldn't exists. Removing ${module_path}"
			rm $module_path
	fi
		
	pushd /linux/tools/labs
		if [ -f $output ]; then
			echo "Removing $output"
			rm $output &> /dev/null
		fi
		if [ -f $finished ]; then
			echo "Removing $finished"
			rm $finished &> /dev/null
		fi

		echo "Building..."
		make build

		if [ ! -f $module_path ]; then
			echo "Cannot find $assignment_mod"
			exit 1
		fi
	
		# copy *.ko in checker
		echo "Copying $module_path into $checker_dir"
		cp $module_path $checker_dir

		LINUX_ADD_CMDLINE="so2=$assignment" ./qemu/run-qemu.sh &> /dev/null &
		
		echo -n "CHECKER IS RUNNING"
		while [ ! -f $finished ]
		do
			if ((timeout >= TIMEOUT)); then
				if [ -f $output ]; then
					cat $output
				fi
				timeout_exceeded
			fi
			sleep 2
			(( timeout += 2 ))
			echo -n .
		done
		cat $output
	popd
}

case $1 in
	0-list)
		run_checker $ASSIGNMENT0_MOD $ASSIGNMENT0_DIR $ASSIGNMENT0_CHECKER_DIR $ASSIGNMENT0_OUTPUT $ASSIGNMENT0_FINISHED $1
		;;
	*)
		usage
		;;
esac
