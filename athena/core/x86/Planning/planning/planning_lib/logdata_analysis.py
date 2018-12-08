#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import re
import matplotlib.pyplot as plt

logFile = r'acc001.log'
with open(logFile,'r') as logdata:
    logStr=logdata.read()
    
pattern_length = re.compile('(?<=delta_length=)[0-9\.]+')
delta_length_oringin = pattern_length.findall(logStr)
length_list = []
for data in delta_length_oringin:
    temp_length = eval(data)
    length_list.append(temp_length)
    
plt.plot(length_list)
plt.show()
min(length_list)
max(length_list)
    
