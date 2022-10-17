from crypt import methods
from multiprocessing.context import ForkProcess
from pickletools import read_unicodestring8
import flask
import flask_table
import os
import sys
import time
from flask import Flask, render_template, request, redirect, url_for, flash
from flask_table import Table, Col
import requests
from socket import *
import hashlib

app = Flask(__name__)

def check_for_traffic_on_port(port):
    os.system("sudo wireshark -k -f \"port " + str(port) + "\"")
    return "wireshark -k -f tcp port " + str(port)
    
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        if request.form['username'] != 'admin' or request.form['password'].hashlib.sha256() != '5f4dcc3b5aa765d61d8327deb882cf99':#password
            return "You are in"
        else:
            return "You are logged in as admin and your password is " + request.form['password']
    if request.method == 'GET':
        return render_template('login.html')

@app.route('/logout')
def logout():
    return redirect(url_for('index'))

@app.route('/listener', methods=['GET', 'POST'])
def listener():
    if request.method == 'GET':
        return render_template('listener.html')
    if request.method == 'POST':
        port = request.form['port']
        if port == "":
            error = 'Please enter a port number'
            return render_template('listener.html', error=error)
        else:
            return check_for_traffic_on_port(port)

@app.route('/traffic', methods=['GET'])
def traffic():
    if request.method == 'GET':
        return check_for_traffic_on_port(8080)

@app.route('/generate_payload',methods=['GET', 'POST'])
def generate_payload():
    if request.method == 'POST':
        if request.form['ip'] == '' or request.form['port'] == '':
            error = 'Please enter all the required fields.'
            return render_template('generate_payload.html', error=error)
        else:
            ip = request.form['ip']
            port = request.form['port']
            payload = os.system("python3 /home/hoaxshell/hoaxshell/hoaxshell.py -i " + ip + " -p " + port)
            return render_template('generate_payload.html', payload=payload)
    return render_template('generate_payload.html')

if __name__ == '__main__':
    app.run(debug=True)
