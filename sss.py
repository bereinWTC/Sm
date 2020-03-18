import MySQLdb
from time import time
from flask import Blueprint,Flask,render_template,request,redirect,url_for,jsonify,session,flash
from werkzeug import secure_filename
import flask_excel as excel
import os
import pyexcel.ext.xls
import pyexcel.ext.xlsx
import base64
sss = Flask(__name__,static_folder='static',template_folder='templates')
ALLOWED_EXTENSIONS = set(['txt', 'pdf', 'png', 'jpg', 'jpeg', 'gif'])

@sss.route('/')
def hello_world():
    return 'Hello World!'

@sss.route('/map',methods=['GET','POST']) 
@sss.route('/map/<mid>',methods =['GET','POST'])
def map(mid="1"):

    width = 350
    height = 350
    mapid = "../static/map/test_map1.jpg"
    if request.method == 'GET':
	
	#connect to the database and get data
        db = MySQLdb.connect(host='localhost', user='root', passwd='adpk0131', db='sss') 
        cursor = db.cursor()
	sql = "select id,x,y,tag from clients where tag = 0"
	sql2 = "select id,x,y,tag from clients where tag = 1"
	sql3 = "select id,x,y,tag from clients where tag = 2"
	sql4 = "select id,x,y,tag from clients where tag = 3"
	vexist = cursor.execute(sql)
	vds = cursor.fetchall()
	bexist = cursor.execute(sql2)
	bds = cursor.fetchall()
	aexist = cursor.execute(sql3)
	ads = cursor.fetchall()
	sexist = cursor.execute(sql4)
	sds = cursor.fetchall()
        db.commit()
	cursor.close()
	db.close()
	

	#create lists for cursors
	vacant=list()
	busy=list()
	abouttoleave=list()
	special=list()
	posts=list()
	
	
        for i in range(len(vds)):
            posts.append({
                'id':vds[i][0],
		'position':'('+str(vds[i][1])+','+str(vds[i][2])+')',
		'status': 'Vacant'
                })
            vacant.append({'name':str(vds[i][0]),'x':float(vds[i][1]),'y':height-float(vds[i][2])})

        for i in range(len(bds)):
            posts.append({
                'id':bds[i][0],
		'position':'('+str(bds[i][1])+','+str(bds[i][2])+')',
		'status': 'Busy'
                })
            busy.append({'name':str(bds[i][0]),'x':float(bds[i][1]),'y':height-float(bds[i][2])})

        for i in range(len(ads)):
            posts.append({
                'id':ads[i][0],
		'position':'('+str(ads[i][1])+','+str(ads[i][2])+')',
		'status': 'AboutToLeave'
                })
            abouttoleave.append({'name':str(ads[i][0]),'x':float(ads[i][1]),'y':height-float(ads[i][2])})

        for i in range(len(sds)):
            posts.append({
                'id':sds[i][0],
		'position':'('+str(sds[i][1])+','+str(sds[i][2])+')',
		'status': 'Special'
                })
            special.append({'name':str(sds[i][0]),'x':float(sds[i][1]),'y':height-float(sds[i][2])})
        
        #abouttoleave = [[300, 295], [260, 295], [160, 295], [300, 250], [260, 250], [160, 250],[300, 190], [260, 190], [160, 190],[300, 135], [260, 135], [160, 135],[300, 90], [260, 90], [160, 90]]
	
	#render the map
        series = [{'name':'vacant','color':'rgba(40, 255, 40, 1.0)','data':vacant,'marker': {
                'radius':16
            }},{'name': 'busy','color': 'rgba(255, 40, 40, .8)','data':busy,'marker': {
                'radius':16
            }},{'name': 'abouttoleave','color': 'rgba(255, 160, 40, .8)','data':abouttoleave,'marker': {
                'radius':16
            }},{'name': 'specialk','color': 'rgba(40, 40, 255, .8)','data':special,'marker': {
                'radius':16
            }}]

        return render_template("map1.html",posts = posts,series=series,mapid =mapid,width=width,height=height,mid=mid)


if __name__ == '__main__':
    sss.run()
