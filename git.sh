#!/bin/bash
git status

echo " "
read -p "Do you want check pull sync with master branch ? (y/n) " yn
case "$yn" in

y) 
	clear
	git pull origin master;
	sleep 1
	read -p "Do you want force push? (y for push or N for commit local) " yN
	case "$yN" in
	        y)
	        	clear
	        	read -p " >> Input commit Message (GITHUB BRANCH): " m
			git add .;
			echo "Add to local branch completed . . ."
			echo ""
			git commit -m "$m"
			echo ""
			echo "Commit completed . . ."
			sleep 2
			read -p " >> Push to github repo? (y/n) " pn
			case "$pn" in
				y)
					clear
					git push origin backend
					exit;;
				n)
					exit;;
			esac
			clear;;
	        n) 
	        	clear
	        	read -p " >> Input commit Message (LOCAL BRANCH): " ml
			git add .;
			echo "Add to local branch completed . . ."
			echo ""
			echo "Commit to local branch with message : $ml"
			git commit -m "$ml"
			echo ""
			echo "Commit completed . . ."
			sleep 1
			clear
	esac;;

n) 
	read -p " >> Input commit Message (LOCAL BRANCH): " ml
	git add .;
	echo "Add to local branch completed . . ."
	echo ""
	echo "Commit to local branch with message : $ml"
	git commit -m "$ml"
	echo ""
	echo "Commit completed . . ."
	sleep 1
	exit;;
esac



