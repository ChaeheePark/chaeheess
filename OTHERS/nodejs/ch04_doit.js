var fs=require('fs');

fs.open('./misson01.txt','r',function(err,fd){
    if(err) throw err;
    
    var buf=new Buffer(200);
    fs.read(fd,buf,0,buf.length,null,function(err,bytesRead,buffer){
        if(err) throw err;
        var array=buf.toString('utf8',0,bytesRead).split('\n');

        for (i in array){
            var info=array[i].split(" ");
            console.log(info[0]);
        }
        
        fs.close(fd,function(){
            console.log('파일 닫기');
        });
    });
});