# -*- coding: utf-8 -*-

import serial
import struct
import time

info_list = [
['1', "8/4号晚上10点到8/6号早上6点停水,请做好储水准备,防止生活用水不足.."],
['2', "8/4号晚上10点到8/6号早上6点停水,请做好储水准备,防止生活用水不足.."],
['3', "8/4号晚上10点到8/6号早上6点停水,请做好储水准备,防止生活用水不足.."]
]

def build_cmd(info_list):
	cmd = ""
	for info in info_list:
		info_content = info[1].decode('utf-8').encode('gb2312')
		c = struct.pack("!chh", info[0], 0, len(info_content)) + info_content
		cmd = cmd + c

	cmd = struct.pack("!Bh", 0xA0, len(cmd))+ cmd
	return cmd

def get_available_com():
	''' 得到可用的com口列表 '''
	com_list = []

	for i in xrange(0, 50):
		try :
			ser = serial.Serial(i)
			ser.close()
			com_list.append((i, ser.name))
		except Exception, e:
			pass

	return com_list


com_list = get_available_com()
print com_list
ser = None
if len(com_list) > 0 :
	ser = serial.Serial(com_list[0][0])

while True :
	for i in info_list:
		i[0] = '1'
	cmd = build_cmd(info_list)
	ser.write(cmd)

	time.sleep(1)
	for i in info_list:
		i[0] = '2'
	cmd = build_cmd(info_list)
	ser.write(cmd)

	time.sleep(1)
	for i in info_list:
		i[0] = '3'
	cmd = build_cmd(info_list)
	ser.write(cmd)

	time.sleep(1)



