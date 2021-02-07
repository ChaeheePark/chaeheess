**Application Layer **

1. HTTP 

2. DNS

3. DHCP : host가 ip주소를 어떻게 얻는 지에 관한 protocol, dynamic 하게 그때그때 ip주소 받음 

   - Dynamic assignment 선호하는 이유
     - addresses are assigned on-demand
     - avoid manual ip configuration
     - support mobility of laptop
   - Broadcast: 한 호스트에서 네트워크 상의 모든 호스트들에게 패킷을 보내는 과정
     - directed broadcast : 특정 네트워크 상의 모든 호스트들에게 패킷을 전송하는 방법
     - local broadcast : 로컬 네트워크 상에 존재하는 호스트들로만 broadcast

   - DHCP (동적 주소할당 프로토콜): discover - ip address 요청, offer- ip address , offer, request, ack (서술형으로풀어쓰기)

     - router의 interface가 서로 다른 subnet에 속함 (router가 속한 subnet은 없음!!)
     - message format : opcode- 1은 request, 2는 reply -> option에 어디 속하는지 나와있음
     - DHCP is an application-layer program, using the client-server paradigm, helps tcp/ip at the network layer

   - How a network gets IP addresses ? -> Network gets allocated portion of its provider ISP's address space, 그렇다면 ISP는 어떻게 address블럭을 얻냐면 수작업으로!


      4. Socket programming==application programming
         - client / server model : server는 client에 대한 정보는 없어도 되지만 client는 server에 대해 ip address, port number를 알고 있어야 함
         - socket-address : ip address + port number (client와 server을 연결해주는 interface)
         - server :다양한 server process존재(FTP, HTTP등 ), 머신이 꺼질때 까지 계속적으로 run한다.
           - What is Socket? -> application process와 OS process의 접점  
           - socket 은 IP address 와 Port 넘버가 합쳐진, 네트워크 상에서 서버 프로그램과 클라이언트 프로그램이 양방향 통신을 할 수 있도록 해주는 application programming interface이다.
