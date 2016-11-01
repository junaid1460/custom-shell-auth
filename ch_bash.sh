
exist=`grep -P "custom-login" ~/.bashrc`
if [[ ${#exist} -gt 0 ]] ; then
	echo "Making no modification to ~/.bashrc"
else
	echo "Setting up auto login"
	echo "$1 \$\$" >> ~/.bashrc
fi
