var result=0;
console.time('duration_sum'); //시작시간 기록
for (var i=1; i<=1000; i++){
    result+=i;
}

console.timeEnd('duration_sum'); //끝시간 기록
console.log('1 부터 1000까지 더한 결과물: %d',result);