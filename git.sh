while getopts m: flag
do
    case "${flag}" in
        m) message=${OPTARG};;
    esac
done
git add .
echo "Add to local branch completed . . .";
sleep 1
git commit -m "$message"
echo "Commit to local branch with message : $message"
echo "Commit completed . . ."
sleep 2
cls
