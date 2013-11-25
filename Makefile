all:compile
debug:
	g++ main.cpp -o api -ltesseract -llept -g -lcurl -DDEBUG
compile:
	 g++ main.cpp -o api.cgi -ltesseract -llept -g -lcurl
install:compile
	cp /home/zodiac1111/tmp/tessapi/api.cgi /var/www/cgi-bin/api.cgi
clean:
	rm -f api api.cgi