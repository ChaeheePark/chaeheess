**Network Layer**

1. Datagram networks

- ip fragmentation, reassembly
  - fragmentation : identification 에 fragment 된 패킷들 다 같은 아이디로 기록, offset에 앞의 fragment 의 길이를 기록하고 final destination에서 재조합
  - 왜필요? layer 마다 MTU가 다른데 ip packet이 network layer 의 mtu가 더 크기때문에 fragment해서 데이터를 전송해야함
- MTU : IP 데이터 그램이 네트워크를 통해 전송될 때, 전송되는 IP 데이터 그램의 크기가 해당 전송 매체에서 전송될 수 있는 최대 크기

- network layer functions
  - packetizing(encapsulating the payload , decapsulating the payload), routing(경로를 찾아주는 행위) and forwarding(적절한 interface로 packet을 내보내줌), Connection setup(packet이 전송되기 전 두 호스트와 라우터들 간에 virtual connection을 생성), Packet switching
  - routing algorithm: determines end to end path through network
  - forwarding table  : determines local forwarding to an output port(내보냄)
- Routing algorithm
  - global : 모든 information(사전작업으로) 모아 서로 다 주고받게
  - decentralized : 내 주변 link information만
    - 다익스트라 
    - 벨맨포드
    - updating path-vectors
  - Internet structure
    - 내부 routing use 다익스트라, 밸맨포드 , 외부 routing use updating path vectors사용함
- IP Protocol : 모든것을 통합해줌
  - ip provides
    - Unreliable : ip는 lost packet을 다시 회복하려하지 않는다.
    - connectionless
    - best effort : no throughput guarantee, no delay guarantee
  - ip packet format
    - DS/ECN field : DS는 요구되는 서비스 질에 대한 유형을 나타내는데 사용, ECN는 혼잡 알림을 위해 사용
    - TTL : 패킷의 생존 시간을 설정함, 0이되면 버리고 발신지로 icpm패킷을 보내준다
    - 
- ICMP : TCP/IP에서 Ip 패킷을 처리할때 발생되는 문제를 알려주는 프로토콜, network layer protocol 이지만 ip packet에 처음으로 encapsulation됨

  - 에러메세지종류 - Error-reporting / Query

    - Destination - unreachable : Protocol/port created by host and all others are created by a router
- ICMP error messages : first 8 bytes of the Transport layer header is included, provide port num ,sequence num
  - Source Quench : 네트워크상의 통신량이 폭주하여 목적지 또는 라우터등의 메모리나 버퍼용량을 초과하여 ip 데이터그램이 유실되는 상태가 되면, ICMP Source Quench 에러메세지를 송신측에 통보하여 혼잡제어의 역할을 함 (type=4)
  - ICMP Redirection : 송신호스트로부터 패킷을 받은 수신 라우터가 특정 목적지로 가는 더 짧은 경로를 host에게 update할때 알리고자 하는 메세지 
  - traceroute port unreachable 보내는 이유: 정리하기..
