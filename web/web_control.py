#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16
from std_msgs.msg import String
from signal import signal,SIGINT
import sys

from flask import Flask
from flask import render_template
from flask import request

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route("/hip")
def setHip():
    value = int(request.args.get("value"))
    rospy.loginfo("Setting Hip Position value to {0:d}".format(value))
    hip_pub.publish(value)
    return "Hip value {0:d}".format(value)

@app.route("/thigh")
def setThigh():
    value = int(request.args.get("value"))
    rospy.loginfo("Setting Thigh Position value to {0:d}".format(value))
    thigh_pub.publish(value)
    return "Thigh value {0:d}".format(value)

@app.route("/shin")
def setShin():
    value = int(request.args.get("value"))
    rospy.loginfo("Setting Shin Position value to {0:d}".format(value))
    shin_pub.publish(value)
    return "Shin value {0:d}".format(value)

@app.route("/gait")
def setGait():
    value = request.args.get("value")
    rospy.loginfo("Setting Gait to '{0:s}'".format(value))
    gait_pub.publish(value)
    return "Current Gait: {0:s}".format(value)

def initRospy():
    global hip_pub
    global thigh_pub
    global shin_pub
    global gait_pub
    rospy.init_node('web_control', anonymous=True)

    hip_pub = rospy.Publisher('/left/front/hip/command', Int16, queue_size=10)
    thigh_pub = rospy.Publisher('/left/front/thigh/command', Int16, queue_size=10)
    shin_pub = rospy.Publisher('/left/front/shin/command', Int16, queue_size=10)
    gait_pub = rospy.Publisher('/gait_command', String, queue_size=10)

def sigintHandler(signal, frame):
    rospy.loginfo("Terminating webserver")
    sys.exit(0)

if __name__ == '__main__':
    signal(SIGINT, sigintHandler)
    initRospy()
    print "Starting webserver"
    app.run(debug=True, host='0.0.0.0', use_reloader=False)
