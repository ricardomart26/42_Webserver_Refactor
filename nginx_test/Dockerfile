FROM nginx:latest

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install vim -y
RUN apt-get install net-tools -y
RUN apt-get install php-fpm -y

COPY ./nginx.conf /etc/nginx/ 
COPY ./default.conf /etc/nginx/conf.d/

RUN mkdir -p /etc/nginx/logs
RUN touch /etc/nginx/logs/{error,access}.log
# RUN chown -R www-data:www-data /var/Www/html/
COPY ./web_files /var/www/html/web_files
RUN chmod 777 /var/www/html/
RUN chown -R www-data:www-data /var/www/html/web_files

RUN nginx -t
RUN sed -i "s/run\/php\/php7.4-fpm.sock\//127.0.0.1:9000/g" /etc/php/7.4/fpm/pool.d/www.conf
RUN service php7.4-fpm start

ENTRYPOINT ["nginx", "-g", "daemon off;"]
