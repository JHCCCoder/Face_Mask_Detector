#!/usr/bin/python
# -*- coding: utf-8 -*-
from flask import Flask

app = Flask(__name__,
template_folder='templates',
static_folder='static',)
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:cjh123456@127.0.0.1/flaspp'
app.config['SQLALCHEMY_COMMIT_ON_TEARDOWN'] = True  #链接数据库
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True#链接数据库
app.config['SECRET_KEY'] = 'a20203233'
