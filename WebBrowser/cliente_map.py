import gpxpy
import gpxpy.gpx
import folium
import MySQLdb
import time as ti
import datetime
import time

from math import sin, cos, sqrt, atan2, radians
import sys
import time
from PyQt4.QtGui import QApplication
from PyQt4.QtCore import QUrl
from PyQt4.QtWebKit import QWebView



def savemap():

	db = MySQLdb.connect(host="localhost",  # your host 
						user="root",       # username
						passwd="cidte",     # password
						db="GPS")   # name of the database
		


	# Create a Cursor object to execute queries.
	cur = db.cursor()
 
	# Select data from table using SQL query.
	cur.execute("SELECT * FROM gprmc")
 
	# print the first and second columns      
	i=0
	for row in cur.fetchall() :
		gp= row[1];
		print gp
		
		if  gp[0]=='$'and gp.find('*')>=0 and (gp[30]=='S' or gp[30]=='N') and gp[18]=='A' and (gp[43]=='W' or gp[43]=='E') and gp[18]=='A':
			print "****" 
			time_gp = float(gp[7:12])
			lat=float(gp[20:29]) 
			lon=float(gp[32:42]) 
			lat=lat/100
			lon=lon/100
			lat=((lat- int(lat))*5/3+int (lat))
			lon= ((lon - int(lon))* 5/3+ int (lon))


			if gp[30]=="S":
				lat=-lat
			if gp[43]=='W':
				lon=-lon 
				
			if i==0:	
				map_cliente = folium.Map(location=[lat, lon],
							zoom_start=10,
							tiles='Stamen Terrain')
			folium.Marker([lat, lon], popup='Cliente').add_to(map_cliente)
			i=i+1	
			map_cliente
			map_cliente.save("./recorrido_cliente.html")	

savemap()			


			
		
		

		
		

 

	
	
