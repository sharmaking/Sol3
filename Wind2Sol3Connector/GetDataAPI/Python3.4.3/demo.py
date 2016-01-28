# -*- coding: utf-8 -*-
#导入类库
from Api import GetDataCallBack, GetDataApi

#自定义的回调函数 继承至 GetDataCallBack
class MySpi(GetDataCallBack):
	def __init__(self):
		super(MySpi, self).__init__()
		pass

	#代码表回调, 登录成功即会触发
	def OnRecvCodes(self, codes, optionCodes):
		print("OnRecvCodes:")
		#for code in codes:
		#	print(code)
		print(len(optionCodes))

	""" 
	实时、全真模拟行情数据回调 
	"""
	#实时或模拟数据每日切换时机会触发
	def OnRecvDayBegin(self, dateStr):
		print("OnRecvDayBegin:", dateStr)
		pass

	#股票、期货、期权行情数据回调
	def OnRecvMarket(self, market):
		#print("OnRecvMarket:", market["szType"], market["szWindCode"], market["szDatetime"])
		pass

	#指数行情回调
	def OnRecvIndex(self, index):
		#print("OnRecvIndex:", index["szWindCode"], index["szDatetime"])
		pass

	#股票、期货、期权 逐笔成交回调
	def OnRecvTransaction(self, transaction):
		#print("OnRecvTransaction:", transaction["szWindCode"], transaction["szDatetime"])
		pass

	#实时或模拟数据每日收盘或当日数据接收完成后触发
	def OnRecvDayEnd(self, dateStr):
		print("OnRecvDayEnd:", dateStr)
		pass

	""" 
	历史k线查询回调 
	"""
	#k线数据
	def OnRecvKLine(self, kLine):
		print("OnRecvKLine:", kLine["szWindCode"], kLine["szDatetime"])
		pass

	#数据查询结束
	def OnRecvOver(self):
		print("OnRecvOver:")
		pass


def main():
	spi = MySpi()
	api = GetDataApi.GetDataApi(spi)

	#连接服务器并登录
	api.Connect("116.228.204.37", 18500)
	api.Login("WanTest", "WanTest")

	#生产待订阅股票列表		600000 与 600000.SH 通用
	subcode = ["600000", "000300", "IFC1"]

	#请求实时数据示例
	#api.ReqRealtimeData(subcode, False)

	#请求历史数据回放示例
	api.ReqSimulationData("2015-12-18 9:00:00", "2015-12-18 9:35:00", subcode, False);

	#请求K线数据示例
	#api.ReqKline("minute", "2015-12-3 9:0:0", "2015-12-4 9:45:0", subcode, False);

	#主线程进入阻塞，等待辅助线程接收数据
	while True:
		pass
	pass

if __name__ == '__main__':
	main()