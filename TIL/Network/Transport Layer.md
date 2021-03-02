**Transport Layer**

1. socket programming 

- multiplexing / demultiplexing at Transport layer
  - demultiplexing : transport layer에 있는 여러 segments를 적절한 socket으로 보내주는 것

  - Multiplexing : 여러 소켓으로부터 data chunks를 모아서 각 data chunk마다 header 붙여주고 암호화해서 segment로 만들고, network layer에 전달하는 것
- connectionless demultiplexing
  - transport layer 다중화에서 두가지 요구사항:
    - 소켓은 유일한 식별자를 가진다.
    - 각 세그먼트는 세그먼트가 전달될 적절한 소켓을 가리키는 특별한 필드를 가진다(출발치 포트번호와 목적지 포트번호)
  - 호스트의 각 소켓은 OS로 부터 포트번호를 할당받는다. 그리고 세그먼트가 호스트에 도착하면 transport layer은 세그먼트 안의 목적지 포트번호를 검사하고 상응하는 소켓으로 세그먼트를 보내고 데이터는 소켓을 통해 해당하는 프로세스로 전달이된다.
  - udp socket들은 destination IP address와 destination port number로 구분된다. IP datagram이 다른 host에서 왔더라도 dest ip와 dest port가 같으면 같은 소켓으로 들어간다. UDP를 사용하는 application이 서로 다른 source에서 왔다는 것을 구분해줘야 한다. =>destination port number, ip만 같다면 모두 같은 것
- connection-oriented demux
  - source, destination socket address (ip,portnumber) 4가지로 tcp connection을 구분한다.
  - server에서 listening socket으로 connection이 들어오면 3 way handshake후 connection socket이 만들어진다. 그래서 client 하나 생길 때마다 connection socket을 계속 만든다.
- UDP다른점
  - connection 없음, sendto() recvfrom 



2. Reliable Data Transfer Protocol

- Propagation delay & Transmission delay

  - Transmission delay : 하나의 패킷이 link에 transmission 하는데 걸리는 시간, packet이 얼마나 두껍냐에 따라 달라짐(frame의 크기, link의 speed)
  - Propagation delay : 거리에 따라 걸리는 시간 (거리와 구리선로의 속도)

- Basic error control : Stop - And - Wait ARQ (Packet error)

  - error가 있으면 ACK 패킷 안옴 - > frame을 재전송 해야함 (timer로 시간 판단 후 일정시간동안 ACK패킷 안왔다고 생각하면 frame을 재전송)

  - max data rate depends on RTT, frame length

  - 에러를 판단할때 일정한 시간을 기다려야 하므로 너무 느리다 

  - 시간을 빨리하려면 - > frame length를 길게하면 되지 않을까? 

  - Pipelining : ACK를 반환하지않고 window size만큼 frame을 보낸다 - > 그리고 ACK를 기다린다(누적ACK)

  - window size는 왜 2^m-1? -> 최악의경우, frame이 모두 잘갔는데 receiver로 부터 ACK가 다 안왔을때, frame의 sequence number가 보냈던 window에 속한 frame이 아닌 새로운 sequence number의 data가  

  - window 사이즈 다음의 데이터와 윈도우 사이즈에 해당하는 0이 같은 시퀀스 넘버라면 윈도우 사이즈 가 2^m이 되었을때 **receiver로부터 data가 다 안오게 되면** sender측에서는 **window size가 넘어선 숫자**를 보낼수 있다.

    


3. TCP (한쌍의 socket간의 통신 책임)

- TCP Error control : based Go-back-N ARQ -> 왜 이렇게 부를까? ACK는 특정 frame을 받도록 설정되어 있는데 그 frame을 못받으면 , 즉 error가 발생하면 timeout이 생긴다. 그러면 다시 특정 n번째 frame으로 돌아가서 frame을 재전송 해야한다. 이것이 다시 돌아가서 N번째 frame을 보내는 것으로 go-back-n ARQ라고 한다.

  - GO back N ARQ와 다른점 : tcp send window & receive windows are variable size 그래서 sequence number의 시작 byte number로 얼마만큼 띄어져 있구나 확인할 수 있다. 1) 특정 loss packet만 채움 2) byte oriented 3) window size variable size
    - N (window size) 만큼의 Packet 을 한번에 보낸다.
    - ACK(n) 이 오면 Seq# n까지 잘 받았다는 의미이다 = Cumulative ACK
    - 타임아웃이 발생하면 timout(n), n이상의 window size 만큼 재전송한다.
  - ACK 의 두가지 의미 : segment를 잘 받았다, segment를 처리 완료했다(상위 protocol에게 전달을 마쳤다)
  - timeout을 얼마나 오래 기다려야 하는지(timeout 시간을 어떻게 정하는지): 추정된 RTT로 정함 (측정해서 내가 보낸 frame을 얼마나 빨리 ACK해주느냐! 하지만 때에따라 달라서 weighted avg구함)
  - timeout을 많이 기다리는것은 오래걸림 - > duplicate ACK(ACK을3번보내면 잘못왔다) 기대되는 frame의 id가 아닐때 기대하는 frame id 로 ACK를 3번보냄 

- Transmission Rate Control (전송속도제어) with a window: window size를 적당한 크기로 정해줘야함 -> 작으면 충분한 pipelining 을 못해 효율이 떨어지고 크면 여유가 없어서 문제발생 가능성이 커진다(congestion)

  - 전송속도 bps = windowsize * 상수c / RTT
  - TCP window control을 해야한다!

- TCP Window Controls

  - Flow control : receiver가 주체적으로 control, receiver가 사용가능한 buffer의 크기를 sender에게 알려줘야함! receive window 크기까지만 최대로 보내야함!!
- Congestion control : input으로 너무 많은 data가 올때 network의 overload를 sender가 결정해줘야한다. 그래서 cwnd를 늘이고 줄여야한다

  - congestion 을 알아내는 방법 1) packet loss  (특수한 경우 buffer에 쌓이는 packet이 많으면 packet loss가 됨 )2) delay (하지만 delay된건지 안된건지 정확하게 알 수 없다 ), 3)explicit mark (중계기가 segment에 mark)

  - causes/costs of congestion:

    - 1) router가 무한한 크기의 버퍼를 갖는 경우: 지연시간이 매우 증가하므로 혼잡시 큰 지연이 발생한다. 
      - 2) router가 유한한 크기의 버퍼를 갖는 경우 : 송신측에서 타임아웃된 패킷의 재전송이 이루어진다. 즉 큐가 유한할경우 패킷의 손실이 발생한다.
      - 3) 모든 연결된 network 장비를 거쳐가는 경우 : 재전송 발생 증가하고 패킷이 경로상에 버려지는 상황이 증가하면 상위 라우터에서 사용되는 전송용량이 낭비되 congestion발생한다.
      - 전송시간 초과에 따른 타임아웃 통해 패킷들이 재전송됨 , 혼잡 초기에 전송지연이 점점 증가할때, 타임아웃갑싱 너무 작으면 혼잡도가 급격히 증가하고 네트워크에 혼잡징후가 보이면 이를 감지에 타임아웃을 늘려야함. 패킷을 버릴때 패킷 재전송현상으로 혼잡도가 증가, ACK 알고리즘이 혼잡도 미침, 네트워크에서 전송중인 패킷은 무한정 라우팅되지 않아서 생존시간을 지정해서 시간 초과를 하지 않도록 설계
      - congestion의 심각성은 ? 

  - congestion control : 

    - 1) additive increase :  송신측에서 ACK를 못받고 타임아웃 되면 패킷손실이 발생한 것으로 알고 congestion으로 받아들여서 송신측은 cwnd를 줄여 데이터 전송 속도를 조절한다. 송신측에서 패킷의 손실이 발생할 때 까지 이용가능한 대역 폭을 탐지하면서 전송률 cwnd를 증가시킨다. (addictive increase : 손실이 발생할 때 까지 증가 / 손실이 발생한 후에 cwnd를 절반으로 감소 ) .
    - 2) TCP Tahoe:  timeout이 발생했을때 무조건 1로 떨어트림, slow start로 증가하다가 타임아웃에 의해 loss가 발생되면 , cwnd를 1로 떨어트린다. 그리고 treshold를 loss가 발생했을때 cwnd 의 절반 값을 설정한후 거기 까지 가면 1로 다시 설정한다.
    - 3) TCP Reno: timeout에 의한 loss는 cwnd는 절반으로 감소하고 3 duplicate ACK은 loss가 발생한 시점의 window값의 절반으로 떨어트린다. (slow start로 증가)

- 결국 window 크기는 cwnd와 rwnd의 min값



4. UDP



