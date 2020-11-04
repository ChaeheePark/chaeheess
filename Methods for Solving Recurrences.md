Methods for Solving Recurrences

1) Substitution method : 수학적 귀납법

T(n) = 2T(n/2) + n라는 식이 주어졌을때 T(n) = O(nlogn)라고 추론할 수 있다.

그렇다면 T(n)<=c*nlogn이 된다.

T(2)=2T(1)+2=4이고, T(2)=c*2 *log2=2c이므로 c>=2에서 성립한다.

T(n) = 2T(n/2) + n

​	<= 2 * cn/2log(n/2) + n

​	= cnlogn - cnLog2 + n

​	= cnlogn - cn + n (c>=2이므로 (-)되어 사라짐)

​	<= cnlogn



2)Recursion Tree : 각 node의 비용과 level을 더해서 재귀트리의 총 비용을 구함

![image-20200919125936540](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20200919125936540.png)

 	

![image-20200919125957849](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20200919125957849.png)



3) Master Theorem

T(n)=aT(n/b)+f(n) 

 	c1) f(n)>n^logba이면 T(n)=Θ(f(n))

​	c2) f(n)=n^logba이면 T(n)=Θ(n^logba*logn)

​	c3) f(n)<n^logba이면 T(n)=Θ(n^logba)



​	

