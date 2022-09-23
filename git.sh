#!/bin/bash
git status

echo " "
read -p "Do you want check pull sync with master branch ? (y/n) " yn
case "$yn" in

y) 
	git pull origin master;
	sleep 1
	read -p "Do you want force push? (y for push or N for commit local) " yN
	case "$yN" in
	        y)
	        	read -p "Input Message push repo : " m
			git add .;
			echo "Add to local branch completed . . ."
			echo ""
			echo "Commit to local branch with message : $m"
			git commit -m "$m"
			echo "Commit completed . . ."
			sleep 3
			clear;;
	        n) 
	        	clear
	esac;;

n) 
	exit;;
esac



