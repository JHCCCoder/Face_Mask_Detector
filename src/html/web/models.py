#!/usr/bin/python
# -*- coding: utf-8 -*-
from app_config import app
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.sql import func

db = SQLAlchemy(app)
# Create a database model
class Info(db.Model):
    __tablename__ = 'userinfo'
    id = db.Column(db.Integer, primary_key=True)
    status = db.Column(db.Integer)
    createTime = db.Column(db.DateTime(timezone=False), server_default=func.now())
    def __repr__(self):
        return '<Info %r>' % self.status
