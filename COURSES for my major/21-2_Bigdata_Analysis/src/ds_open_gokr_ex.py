#!/usr/bin/env python
# coding: utf-8
import os
import requests
import urllib
import mylib

def doIt():
    # (1) key
    keyPath=os.path.join(os.getcwd(), 'src', 'key.properties')
    key=mylib.getKey(keyPath)
    keygokr=key['gokr'] # keygokr='8Bx4C1%2B...'
    # (2) service + operation
    SERVICE='ArpltnInforInqireSvc'
    OPERATION_NAME='getMinuDustFrcstDspth'
    params1=os.path.join(SERVICE,OPERATION_NAME)
    # (3) query params encoding
    _d=dict()
    _d['searchDate']='2021-08-10'
    params2 = urllib.parse.urlencode(_d)
    # (4) make a full url
    params=params1+'?'+'serviceKey='+keygokr+'&'+params2
    #OLD: _url='http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc'
    _url='http://apis.data.go.kr/B552584'
    #url=urllib.parse.urljoin(_url,params)
    url="/".join([_url,params])
    # (5) get data
    data=requests.get(url).text
    print (data)

if __name__ == "__main__":
    doIt()
