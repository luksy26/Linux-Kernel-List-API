#!/bin/sh

# THIS SCRIPT RUNS INSIDE THE SO2 VM

ASSIGNMENT0_CHECKER=/home/root/skels/assignments/0-list-checker
ASSIGNMENT0_OUTPUT=/home/root/skels/0-list-output
ASSIGNMENT0_FINISHED=/home/root/skels/0-list-finished

assign0_list()
{
        cd $ASSIGNMENT0_CHECKER
     	   sh _checker &> $ASSIGNMENT0_OUTPUT
       	   echo FINISHED &> $ASSIGNMENT0_FINISHED
        cd -
}

start()
{
        local arg=$(cat /proc/cmdline | grep -o 'so2=[^ ]*' | cut -d= -f2)
        case "$arg" in
                0-list)
                        assign0_list
                        ;;
                *)
                        echo "Unknown option"
                        exit 0
                        ;;
        esac
}

# Carry out specific functions when asked to by the system
case "$1" in
        start)
                echo "Starting so2_vm_checker_daemon.sh..."
                start
                ;;
        stop)
                echo "Stopping so2_vm_checker_daemon.sh..."
                sleep 2
                ;;
        *)
                echo "Usage: /etc/init.d/foo {start|stop}"
                exit 1
                ;;
        esac

exit 0

