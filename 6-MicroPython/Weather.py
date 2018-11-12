import network
import time
import json
import urequests
import machine

sta = network.WLAN(network.STA_IF)
sta.active(True)
sta.connect("SSID", "PASSWORD")
sta.ifconfig()
time.sleep(5)

def monitor():
	r = urequests.get("http://apidev.accuweather.com/currentconditions/v1/211298.json?language=en&apikey=hoArfRosT1215").json()
	suhu = r[0]['Temperature']['Metric']['Value']
	waktu = r[0]['LocalObservationDateTime']
	jam = waktu[11] + waktu[12] + waktu[13] + waktu[14] + waktu[15]
	print("Pada jam:", jam)
	print("Suhu di Medan adalah", suhu, "C")
	time.sleep(1)
while True:	
	monitor()
	
