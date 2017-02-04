locationFinder:
	gcc -Wall src/main.c src/tools.c src/query.c src/error.c -Iinc/ -o locationFinder -lcurl

clean:
	rm -f locationFinder

