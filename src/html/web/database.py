#!/usr/bin/python
# -*- coding: utf-8 -*-
from app_config import app
from models import db
from flask_migrate import Migrate,MigrateCommand
from flask_script import Manager

manager = Manager(app)
#The first parameter is the instance of Flask and the second parameter is the SQLAlchemy database instance
migrate = Migrate(app,db)
#manager is an instance of Flask-Script, this statement adds a db command to flask-Script
manager.add_command('db',MigrateCommand)

if __name__ == '__main__':
    manager.run()