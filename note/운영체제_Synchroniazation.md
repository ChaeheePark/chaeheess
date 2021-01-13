**Synchronization (9주차)**

- 다중 프로그래밍 시스템 
  - 여러개의 프로세스들이 존재하는데, 만일 함께 사용하는 데이터/파일이 있으면 충돌이 일어날 수 있음 -> 동기화 (Synchronization)
  - shared data : 여러 프로세스들이 공유하는 데이터
  - critical section : 공유데이터를 접근하는 코드 영역
  - mutual exclusion : 둘 이상의 프로세스가 동시에 critical section에 진입하는 것을 막는것
  - 1) mutual exclusion : 쓰고있으면 다른 아이들은 못들어오게
  - 2) Progress : 누군가 들어오길 기다리고있다면 대기시간이 길어지지 않게
  - 3) Bounded waiting : 들어오기 원하는 것이 있으면 허용하되 허용횟수를 정하게



- SW 측면 - Peterson's Algorithm

  - 1) 서로 배타적이여야한다
  - 2) 임계영역에 어떤 쓰레드의 접근도 없을 때 접근이 가능해야 한다.
  - 3) 무한정 대기가 없어야 한다.

- HW 측면 - TAS
- 실행중에 interrupt를 받지 않는 것을 보장


- 3개 이상의 process가 존재하는 경우 bounded waiting 조건을 만족시키지 못함- > compare and swap을 통한 해결
- ->busy waiting 문제



- OS 측면 - mutex 
  - acquire lock, release lock 해서  OS자체에서 잠그고 푼다 -> Busy waiting 문제

- -semaphore
  - 임의의 s변수 하나에 ready queue 하나가 할당(대기실)
  - Counting semaphore / Binary semaphore 
  - 실행순서를  정해서 동기화 가능! 
  - no busy waiting, 그러나 ready queue에서 starvation 일어날수도

--low level mechanism --

--high level mechanism--

- Language - Level 측면 - Monitor
  - 사용이 쉽고, deadlock 등  error발생가능 적음


정리 

개념을 기억, 여러개를수행하면 공통으로 쓰는 데이터 보장, 프로세스 간의 순서를 design 필요, 그래서 lock개념씀 lock을 프로세스간 협동해서 , 주의 깊은 design으로! mutex 랑 semaphore이 중요!! 
