**CPU Scheduling (8주차)**\\



준비단계에 있는 process들을 CPU에 언제 실행시켜 줄것인가에 관한 스케줄링!



- CPU burst(실행시간)은 짧음
- 선점형 스케줄링 : CPU가 어떤 프로세스에 의해 점유중일때 우선순위가 높은 프로세스가 CPU를 차지할 수 있다, 선점이 일어나면 오버헤드가 발생하여 처리시간을 예측하기 힘들다, I/O요청, 응답, Interrupt, 작업완료 시점등
  - SRT : SJF에 선점정책을 도입
  - RR 
  - 다단계큐 : 각 프로세스의 중요도에 따라 큐로 나누고 각 큐에서 다른 알고리즘을 적용해 효율을 높일 수 있다. 하지만 해당 큐에 들어가면 변경이 거의 불가능하다. 
  - 다단계 피드백 큐 : 프로세스가 큐 사이의 이동이 가능하다.(aging기법으로 starvation 방지)
- 비선점형 스케줄링 : 프로세스가 스스로 CPU를 놓아주는 시점(작업이 완료되는 시점)
  - FCFS
  - SJF : 최단CPU burst 시간을먼저 , waiting time 짧아짐
  - 우선순위 스케줄링
- process를 cpu에 올리는것  :dispatch, dispatch latency 발생할수도 (멈추고 다른것을 실행시키게 하는 과정에서)
- CPU utilization, Throughput +
- Turnaround time, waiting time, response time -





- 스레드 : 하나의 프로세스내에서 실행되는 하나 또는 여러개의 작업단위, 프로세스가 실행된다 == 스레드의 흐름

- 멀티 프로세서 스케줄링 : 

  - 프로세싱기법
    - Asymmetric multiprocessing : 하나의 master processor가 다른 processor를 관리감독하는 형태
    - symmetric multiprocessing : 각각의 processor가 각자 스케줄링
  - 메모리접근기법
    - 각각 메모리에 프로세서 다로 담김 : 근데 다른프로세서 메모리 참조할때 ? 매우느리다
    - 그래서 processor affinity(처리기에 친숙한 프로세스를 process에 할당) 와 load balancing(균등하게 처리기에게 프로세스를 push/pull하도록)기법사용
  - 멀티코어 : memory stall해결
  - 쓰레드 vs 코어 :  쓰레드는 논리적 단위, 코어는 물리적 단위의 일 (프로세스안에서)

- 실시간성 스케줄링

  - 1) minimizing latency : latency 를 최소화 해야한다! (interrupt processing, context switch)

  - 2) priority based scheduling : task는 반드시 규칙적인 주기에 따라 실행되어야 한다. 

  - 3) rate monotonic scheduling : 정적우선순위의 선점형 알고리즘

  - 4) EDF : 우선순위를 동적으로 변경하여 deadline이 가장 빠른 process에게 높은 우선순위를 부여

    - soft: 우선순위가 높은거 보장 (realtime보장x), 마감시간 보장하지 않는다(priority based scheduling)
    - hard: deadline안으로 무조건 들어오는것 보장 (rate monotonic scheduling) EDF : 스케줄링을 마감시간 기준으로 우선순위를 구분 ,

    

    

    - 알고리즘 평가 : 

  결정론적 모델링 / 큐잉모델(little's formula) 

 
