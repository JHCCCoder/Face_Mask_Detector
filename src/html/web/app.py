from flask import render_template, request, redirect, jsonify, session  # flask功能包
from app_config import app  # flaskapp
from models import Info, db
import time


@app.route('/api1')
def api1():
    result = db.session.query(Info).all()
    list1 = list()
    for i in result:
        try:
            timeArray = time.strptime(str(i.createTime), "%Y-%m-%d %H:%M:%S")
            timeStamp = int(time.mktime(timeArray))
            list1.append([timeStamp * 1000, i.status])
        except:
            list1.append([str(i.createTime), i.status])
    return jsonify({'data2': list1})


@app.route('/api2')
def api2():
    result1 = db.session.query(Info).filter_by(status=1).count()
    result0 = db.session.query(Info).filter_by(status=0).count()
    list1 = list()
    list1.append({'value': result1, 'name': 'mask'})
    list1.append({'value': result0, 'name': 'unmask'})
    return jsonify({'data': list1})


@app.route('/api3')
def api3():
    return jsonify({'src': '/static/1.png'})


@app.route('/dashboard')
def dashboard():
    return render_template('dashboard.html')
@app.route('/api4')
def api4():
    result1 = db.session.query(Info).filter_by(status=1).count()
    result0 = db.session.query(Info).filter_by(status=0).count()
    total = result0+result1
    route = round((result1/total),2)
    return jsonify({'r1': result1,'r0':result0,'t':total,'route':route})
if __name__ == '__main__':
    app.run(debug=True)
