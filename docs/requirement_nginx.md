1.install
 brew install nginx
 apt-get install libfcgi-dev
 apt-get install libcurl4-openssl-dev
 sudo make install json_fastcgi_web_api.h

2.  nginx configuration
    a. enter: etc/nginx/sites-enabled-default
    b. enter: Sudo nano default
    c. modify default file:
    server {
    listen       80;
    server_name  localhost;

        location /favicon.ico {
          return 204;
          access_log     off;
          log_not_found  off;
        } 

        location /sensor/ {
          include        fastcgi_params;
          fastcgi_pass   unix:/tmp/sensorsocket;
        }

        location / {
             root /var/www/html;
             index index.html;
        }
}
d. put html file into /var/www/html

