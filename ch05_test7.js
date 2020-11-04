var http=require('http');

var options={
    host:'www.google.com',
    port:80,
    path:'/'
};

var req= http.get(options,function(res){
    //응답처리
    var resData='';
    res.on('data',function(chunk){
        resData+=chunk;ejfnstjd
    });
    res.on('end',function(){
        console.log(resData);
    });
});

req.on('error',function(err){
    console.log("오류 발생: "+err.message);
});