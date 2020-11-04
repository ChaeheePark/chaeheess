var http=require('http')

//웹서버 객체를 만듭니다.
var server=http.createServer();

//웹서버를 시작하여 3000포트에서 대기합니다
var port=3000;
server.listen(port,function(){
    console.log('웹 서버가 시작되었습니다.: %d',port);
})

//웹서버를 시작
var host='172.20.10.2';
var port=3000;
server.listen(port,host,'50000',function(){
    console.log('웹 서버가 시작되었습니다. : %s,%d',host,port);
})