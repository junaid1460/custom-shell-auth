
$(VERBOSE).SILENT:

login:
	echo "\t*make build"
	echo "\t*make install"
	echo "\t*make uninstall"

.PHONY:build install clean uninstall
build:
	echo "compiling into custom-login"
	gcc -o custom-login login-setup.c

install:
	echo "installing login system"
	sudo cp -rf './custom-login' '/bin/'
	sudo bash ./ch_bash.sh "custom-login"

clean:
	echo "cleaning temporary files"
	rm custom-login

uninstall:
	echo "uninstalling login system"
	sudo rm /bin/custom-login
	sudo rm /tmp/passwd
