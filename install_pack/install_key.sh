if [ -z "$1" ]; then
	echo "IP is not set"
	exit 1;
fi

KEY_FILE=$HOME/.ssh/ev3_rsa

if [ ! -f $KEY_FILE ]; then
	ssh-keygen -t rsa -P "" -f $KEY_FILE
	ssh-add $KEY_FILE
fi

ssh-copy-id -i $HOME/.ssh/ev3_rsa robot@$1
