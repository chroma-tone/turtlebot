#!/bin/bash

# sudo ampy --port /dev/ttyUSB0 mkdir /picoweb
# sudo ampy --port /dev/ttyUSB0 mkdir /templates
sudo ampy --port /dev/ttyUSB0 put picoweb/__init__.py /picoweb/__init__.py
sudo ampy --port /dev/ttyUSB0 put picoweb/utils.py /picoweb/utils.py
sudo ampy --port /dev/ttyUSB0 put templates/squares.tpl /templates/squares.tpl 
sudo ampy --port /dev/ttyUSB0 put example_webapp.py main.py

