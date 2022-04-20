@page web Web Application
Components Used 
Pycharm 2021.3.1
Mysql 5.7 

Installing
The project needs to install flask, pymysqldb, flask_script, flask-migrate, flask_sqlalchemy.

sudo apt-get install python-pip
sudo pip install flask
pip install pymysqldb
pip install flask_script
pip install flask-migrate
pip install flask_sqlalchemy


How to Run
run the web:
python3 app.py

Deploying the database:
python3 database.py db init       
database.py db migrate         