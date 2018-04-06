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

	db = MySQLdb.connect(host="10.1.135.98",  # your host 
						user="samber",       # username
						passwd="cidte",     # password
						db="GPS")   # name of the database
		


	# Create a Cursor object to execute queries.
	cur = db.cursor()
 
	# Select data from table using SQL query.
	cur.execute("SELECT * FROM gprmc")
 
	# print the first and second columns      
	for row in cur.fetchall() :
		gp= row[1];
		gp_=row[2];
	print gp
	print gp_	

	# Create a Cursor object to execute queries.
	cur = db.cursor()
 
	# Select data from table using SQL query.
	#cur.execute("SELECT * FROM gpgga")
 
	# print the first and second columns      
	#for row in cur.fetchall() :
	#	gpa_c= row[1];
	#	gpa_s=row[2];
	
	
	
	
	#if gpa_c.find('*')>=0 and gpa_s.find('*')>=0 and gpa_c[0]=='$'and  gpa_s[0]=='$':
		#altd=float(gpa_s[53:58])-float(gpa_c[53:58])
		#print "Altitud",altd  
	
	
	

	if  gp[0]=='$'and gp.find('*')>=0 and (gp[30]=='S' or gp[30]=='N') and gp[6]==gp[17]==gp[19]==gp[42]==gp[44]==',' and gp[18]=='A' and (gp[43]=='W' or gp[43]=='E'):
		
	
		print "*****"
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
	if  gp_[0]=='$'and gp_.find('*')>=0 and (gp_[30]=='S' or gp_[30]=='N') and gp_[6]==gp_[17]==gp_[19]==gp_[42]==gp_[44]==',' and gp_[18]=='A' and (gp_[43]=='W' or gp_[43]=='E'):		
		print "*****"
		time_gp = float(gp_[7:12])
		
	
		lat_=float(gp_[20:29]) 
		lon_=float(gp_[32:42]) 
		lat_=lat_/100
		lon_=lon_/100
		lat_=((lat_- int(lat_))*5/3+int (lat_))
		lon_= ((lon_ - int(lon_))* 5/3+ int (lon_))
		
		#phi_1=radians(lat)
		#phi_2=radians(lat_)

		#delta_phi=radians(lat-lat_)
		#delta_lambda=radians(lon-lon_)

		#a=sin(delta_phi/2.0)**2+\
		# 	cos(phi_1)*cos(phi_2)*\
		# 	sin(delta_lambda/2.0)**2
			
		#c=2*atan2(sqrt(a),sqrt(1-a))
		#meters=6371000*c                         # output distance in meters
		#km=meters/1000.0
	
		#print "Distancia ",meters
		
		if gp_[30]=="S":
			lat_=-lat_
		if gp[43]=='W':
			lon_=-lon_		
		
				
		
		map_1 = folium.Map(location=[lat, lon],
						zoom_start=16,
						tiles='Stamen Terrain')
		folium.Marker([lat, lon], popup='Cliente').add_to(map_1)
		folium.Marker([lat_, lon_],
              popup='Servidor',
              icon=folium.Icon(color='green')
             ).add_to(map_1)

		#folium.Marker([lat_, lon_], popup='Servidor').add_to(map_1)
		
		map_1

		map_1.save("./recorrido.html")
		
		   

while True:
	savemap()
	



    

