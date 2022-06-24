#!/usr/bin/python
# -*- coding: utf-8 -*-
from flask import Flask

app = Flask(__name__,
template_folder='templates',
static_folder='static',)
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:cjh123456@127.0.0.1/flaspp'
app.config['SQLALCHEMY_COMMIT_ON_TEARDOWN'] = True  #link the database
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True#link the database

