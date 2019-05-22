all: wish

wish: wish.c
	gcc wish.c -o wish

debug: wish.c
	gcc -g $< -o wishg

debugd: wish.c
	docker run -it --security-opt seccomp=unconfined -v "$(PWD)":/home/ debug 
	# gcc /home/$< -g /home/wishg; gdb -tui /home/wishg

clean:
	rm wish wishg
	rm -r wishg.dSYM
