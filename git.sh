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
	        	read -p "Input Message push repo : " m
			git add .;
			echo "Add to local branch completed . . ."
			echo ""
			echo "Commit to local branch with message : $m"
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
	esac;;

n) 
	exit;;
esac



