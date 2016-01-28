#! /usr/bin/env python 
# -*- coding: utf-8 -*- 
from Api import GetDataApi
import json

class GetDataCallBack(GetDataApi.GetDataSpi):
	def __init__(self):
		super(GetDataCallBack, self).__init__()
		pass

	def RecvCode(self):
		jsonStr = json.loads(self.szData)
		self.OnRecvCodes(jsonStr["codes"], jsonStr["options"])
		pass

	def RecvDayBegin(self):
		self.OnRecvDayBegin(self.szData)
		pass

	def RecvMarket(self):
		jsonStr = json.loads(self.szData)
		self.OnRecvMarket(jsonStr)
		pass

	def RecvIndex(self):
		jsonStr = json.loads(self.szData)
		self.OnRecvIndex(jsonStr)
		pass

	def RecvTransaction(self):
		jsonStr = json.loads(self.szData)
		self.OnRecvTransaction(jsonStr)
		pass

	def RecvDayEnd(self):
		self.OnRecvDayEnd(self.szData)
		pass

	def RecvKLine(self):
		jsonStr = json.loads(self.szData)
		self.OnRecvKLine(jsonStr)
		pass

	def RecvOver(self):
		self.OnRecvOver()
		pass

	def OnRecvCodes(self, codes, optionCodes):
		pass

	def OnRecvDayBegin(self, dateStr):
		pass

	def OnRecvMarket(self, market):
		pass

	def OnRecvIndex(self, index):
		pass

	def OnRecvTransaction(self, transaction):
		pass

	def OnRecvDayEnd(self, dateStr):
		pass

	def OnRecvKLine(self, kLine):
		pass

	def OnRecvOver(self):
		pass