# QMIX: Monotonic Value Function Factorisation for Deep Multi-Agent Reinforcement Learning 논문 리뷰



## Abstract

많은 실제 환경에서, **에이전트 팀은 분산된 방식(decentralised 방식)으로 행동** + 글로벌 상태 정보를 사용할 수 있고 통신 제약이 해제되는 시뮬레이션 또는 실험실 환경에서 **중앙 집중식 방식(centralised 방식)으로 에이전트를 학습**

추가 상태 정보에 따라 공동 작업 값을 학습하는 것은 centralised 방식을 활용하는 매력적인 방법이지만, decentralised 정책을 추출하는 최선의 전략은 명확하지 않음

-> centralised end-to-end 방식으로 decentralised 정책을 학습할 수 있는 새로운 가치 기반 방법인 **QMIX**를 소개 (centralised 정책과 decentralised정책 간의 일관성을 보장)



## 1. Introduction

강화 학습(RL)은 로봇 무리 및 자율 주행 자동차의 조정과 같은 다양한 협력적 다중 에이전트 문제를 해결하는 데 도움이 됨

centralised training with decentralised execution 패러다임에 주목

대부분의 RL 방법이 학습하는 action-value 함수를 어떻게 표현하는 것이 중요

-> 에이전트의 행동의 효과를 적절하게 포착하려면 global state와 joint action에 대한 조건을 충족하는 centralised action-value 함수 
$$
Q{tot}
$$
가 있고 이를 각 에이전트가 개별 관찰에 따라 개별 작업을 할 수 있는 decentralised 정책을 추출해야함

- 가장 단순한 방법은 centralised action-value 함수를 포기하고 독립 Q-러닝(IQL)에서와 같이 각 에이전트가 개별 action-value 함수 
  $$
  Q{a}
  $$
  를 독립적으로 학습하도록 함

  그러나, 이 접근법은 각 에이전트의 학습이 다른 에이전트의 학습과 탐구에 의해 교란되기 때문에 에이전트 간의 상호 작용을 명시적으로 나타낼 수 없음

- 다른 방법은, fully centralised state action value 함수
  $$
  Q{tot}
  $$
  를 학습한 다음 이를 사용하여 actor-critic framework인 counterfactual multi-agent(COMA) 정책을 사용

  그러나 on-policy 이므로 sample inefficient하고, 에이전트가 많을경우 fully centralised된 critic를 훈련시키는 것이 비실용적

- 이 두 방법 사이에서, value decomposition network(VDN)가 취하는 접근 방식인 centralised but factored 
  $$
  Q{tot}
  $$
  를 배울 수 있음
  $$
  Q{tot}
  $$
  는 개별 value 함수 
  $$
  Q{a}
  $$
  의 합으로 표현함으로써, decentralised 정책은 단순히 각 에이전트가 
  $$
  Q{a}
  $$
  와 관련하여 조치를 선택하는 것에서 발생

  그러나 VDN은 표현될 수 있는 centralised action-value 함수의 복잡성을 심각하게 제한하고 학습 중에 사용할 수 있는 추가 상태 정보를 무시 

- 본 논문에서는 VDN과 마찬가지로 IQL과 COMA의 양극 사이에 있지만 훨씬 더 많은 종류의 action value function을 나타낼 수 있는 QMIX라는 새로운 접근 방식을 제안->  핵심은 decentralised된 정책을 추출하기 위해 VDN의 구조가 필요하지 않지만, 대신 
  $$
  Q{tot}
  $$
  로 구한 글로벌 argmax가 각각에 대해 수행된 개별 argmax 연산 집합과 동일한 결과를 생성하도록 하면됨

  이때 Qtot와 Qa는 다음과 같은 간단한 monotonicity constraint만 만족하면됨(1)
  $$
  ∂Qtot\over∂Qa
  ≥ 0, 
  ∀a.
  $$
  QMIX는 각 Qa를 대표하는 에이전트 네트워크와 이를 Qtot으로 결합하는 혼합 네트워크로 구성되며, VDN과 같은 단순한 합이 아니라 centralised 정책과 decentralised 정책 간의 일관성을 보장하는 복잡한 비선형 방식으로 구성+동시에, 혼합 네트워크가 양의 가중치를 갖도록 제한함으로써 (1)의 제약을 시행

  결과적으로 QMIX는 에이전트 수에 따라 잘 조정되고 선형 시간 개별 argmax 연산을 통해 decentralised 정책을 쉽게 추출할 수 있는 요인 표현으로 복잡한 centralised action-value 함수를 나타냄



## 2. Related Work

- 멀티 에이전트 시스템을 위한 정책을 찾는 자연스러운 접근 방식은 decentralised value 함수 또는 정책을 직접 학습하는 것 

  independent Q-learning은 Q-learning을 사용하여 각 에이전트에 대한 독립적 active-value 함수를  학습  -> 이 접근 방식을 DQN을 사용하는 심층 신경망으로 확장-> decentralised value 함수를 학습하고 학습 중에 추가 상태 정보를 포함할 수 없는 문제점을 위해 학습 안정화를 어느 정도 다루지만 여전히 문제점

- 반면, joint action 대한 centralised 학습은 자연스럽게 조정 문제를 처리할 수 있고 비정상성을 피할 수 있지만 joint action space가 에이전트 수로 기하급수적으로 증가하기 때문에 확장하기 어려움-> 확장 가능한 centralised 학습에 대한 고전적인 접근 방식에는 global reward 함수를 agent-local term합으로 분해하여 에이전트 간의 조건부 독립을 활용하는 조정 그래프 사용

  Sparse cooperative Q-learning은 coordination graph에서 이러한 종속성을 인코딩하면서 조정이 필요한 상태에서만 coordinate 에이전트 그룹의 작업을 조정하는 방법을 학습하는 표 형식의 Q-learning algorithm

- centralised 학습의 최근의 접근 방식은 실행 중에 훨씬 더 많은 커뮤니케이션을 요구하는 방식: centralised 네트워크 아키텍처를 사용하여 에이전트 간에 정보를 교환하거나, 양방향 RNN을 사용하여 actor-critic 환경에서 에이전트 간에 정보를 교환

- 일부 연구는 fully decentralised execution으로 centralised 학습 설정을 활용하는 hybrid 방식을 개발: COMA는 centralised critic을 사용하여 decentralised 행위자를 학습시키고, 다중 에이전트 credit assignment를 해결하기 위해 각 에이전트에 대한 반사실적 advantage 함수를 추정-> 에이전트별 critic과 함께 centralised actor-critic algorithm을 제시하고 이는 에이전트 수에 따라 잘 확장되지만 centralisation을 완화 ->각 에이전트에 대한 centralised critic을 학습하고 이를 연속적인 액션 공간이 있는 경쟁 게임에 적용 

- decentralised 실행으로 centralised value 함수 학습을 가능하게 하는 가치 분해 네트워크(VDN)를 제안

- 많은 논문에서 심층 다중 에이전트 RL의 벤치마크로서 StarCraft에서 단위 미세 관리를 확립: centralised greedy MDP 및 1차 최적화를 사용하는 알고리즘을 제시 (분산된 실행이 필요하지 않음) 

  최대 5명의 에이전트가 있는 전투 시나리오에서 IQL에 대한 재생 안정화 방법을 평가 하는게 본 논문과 비슷한 연구

  본 논문에서는 StarCraft와 반대로 StarCraft II 학습 환경에서 micromanagement 작업을 구성

- QMIX는 신경망에 의존하여 하이퍼 네트워크를 연상시키는 방식으로 centralised 상태를 다른 신경망의 가중치로 변환



## 3. Background

fully cooperative multi-agent task는 Dec-POMDP로 설명할 수 있음

학습이 centralised이도 불구하고, 실행은 decentralised되며, 학습 알고리즘은 모든 로컬 액션 관찰 기록과 글로벌 상태에 액세스할 수 있지만, 각 에이전트의 학습된 정책은 자체 액션 관찰 기록에만 의존할 수 있음



### 3.1. Deep Q-Learning

deep Q-learning은 θ에 의해 매개 변수화된 심층 신경망을 통해 action-value 함수를 나타냄

<img width="174" alt="image" src="https://user-images.githubusercontent.com/60170358/170675190-7ccde37c-6103-4349-a36b-034e0ca0a9d0.png">

### 3.2. Deep Recurrent Q-Learning

부분적으로 관찰 가능한 설정에서, 에이전트는 전체 수행 관찰 기록을 조건화하여 이점을 얻을 수 있습니다.
RNN을 활용하는 Deep Recurrent Q network(DRQN)를 제안

### 3.3.Independent Q-Learning

다중 agent 학습에서 가장 일반적으로 적용되는 방법은 Independent Q-Learning(IQL)

이 방법은 다중 에이전트 문제를 동일한 환경을 공유하는 동시 단일 에이전트 문제 모음으로 분해

### 3.4. Value Decomposition Networks

Value Decomposition Networks(VDN)는 joint action-value 함수 Qtot를 배우는 것을 목표로 함

<img width="206" alt="image" src="https://user-images.githubusercontent.com/60170358/170677000-13ea1033-adde-4f3d-bfd4-53e8091791bf.png">



## 4. QMIX

QMIX 핵심은 centralized counterpart와 완전히 일치하는 decentralized 정책을 추출하기 위해 VDN의 full factorisation은 필요하지 않음

대신 일관성을 위해 Qtot에 대해 수행된 글로벌 argmax가 각 Qa에 대해 수행된 개별 argmax 연산 집합과 동일한 결과를 산출하도록 함

<img width="243" alt="image" src="https://user-images.githubusercontent.com/60170358/170678918-f42e1686-61bf-4048-a7f0-eb4b4b0bad1b.png">

이를 통해 각 에이전트 a는 Qa와 관련하여 greedy action을 선택하는 것만으로 decentralised 행동을 참여하게 할 수 있음
Qtot와 각 Qa 사이의 관계에 대한 제약 조건을 통해 Monotonicity 적용할 수 있음

<img width="219" alt="image" src="https://user-images.githubusercontent.com/60170358/170679661-ca6cc565-9521-40c3-b753-38d7ee0ad706.png">

![image-20220527192710513](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20220527192710513.png)각 에이전트 a에 대해 개별 값 함수 Qa(μa, ua)를 나타내는 에이전트 네트워크가 하나 있고

(c) agnet network는 각 시간 단계에서 입력으로 현재 개별 관찰 Oat와 이전 agent uat-1을 수신하는 DRQN으로 나타냄

(b) mixing network는 agent network 출력을 입력으로 받아 단조롭게 혼합하여 Qtot의 값을 생성하는 피드포워드 신경망

(a) (5)의 단조성 제약 조건을 적용하기 위해 mixing network의 가중치는 양수로 제한, mixing network의 가중치는 별도의 hyper network에 의해 생성

->각 hyper network는 state s를 입력으로 받아들이고 mixing network의 한 계층의 가중치를 생성, 각 하이퍼 네트워크는single linear layer과 absolute activation 함수를 거쳐 mixing network의 weights이 음이 아니도록 만듬
출력은 적절한 크기의 행렬로 재구성되는 벡터, biase는 동일한 방식으로 생성되지만 음수로 제한되지는 않음

최종 biase는 ReLU 비선형성을 가진 2층 hyper network에 의해 생성

Qtot은  추가 상태 정보에 의존할 수 있기 때문에 상태는 mixing network로 직접 전달되지 않고 hyper network 의해 사용됨(monotonic network의 가중치를 임의의 방식으로 조건화할 수 있으므로 전체 상태를 가능한 한 유연하게 공동 작업 값 추정치에 통합할 수 있음)

QMIX는 아래와 같은 함수로 손실을 최소화하도록 end-to-end로 학습됨

![image-20220527194309495](C:\Users\chaeh\AppData\Roaming\Typora\typora-user-images\image-20220527194309495.png)



### 4.1.  Representational Complexity	

QMIX로 나타낼 수 있는 value 함수 클래스에는 완전히 관찰 가능한 설정에서 agent의 individual value 함수의 non-linear 단조 조합으로 인수화할 수 있는 모든 값 함수가 포함->VDN으로 표현 가능한 linear 단조 값 함수에 따라 확장

그러나 (5)의 제약 조건으로 인해 QMIX는 이러한 방식으로 함수를 나타낼 수 없음

직관적으로, 에이전트의 최상의 동작이 다른 에이전트의 동작에 동시에 의존하는 값 함수는 적절하게 factorise되지 않으므로 QMIX로 완벽하게 표현될 수 없지만 QMIX는 VDN보다 value 함수를 더 정확하게 근사화 할 수 있음

또한 추가적인 상태 정보를 활용할 수 있음



## 5. Two-Step Game

VDN과 QMIX의 표현 복잡성의 효과를 설명하기 위해, 우리는 두 에이전트를 위한 간단한 2단계 협력 매트릭스 게임을 고안

<img width="251" alt="image" src="https://user-images.githubusercontent.com/60170358/170687114-82d0b135-5d83-42f8-9cc6-f2fdf4203914.png">

첫 번째 단계에서 에이전트 1은 두 개의 매트릭스 게임 중 다음 시간 단계에서 수행할 게임을 선택하고, 첫 번째 단계에서 에이전트 2의 작업은 영향을 미치지 않음

두 번째 단계에서는 두 에이전트 모두 위 그림에 표시된 payoff 매트릭스에 따라 조치를 선택하고 전역 보상을 받음

5000회 동안 이 작업에 대해 VDN 및 QMIX를 교육하고 전체 탐색의 한계(= 1)에서 최종 학습 값 함수를 검사

<img width="249" alt="image" src="https://user-images.githubusercontent.com/60170358/170687582-11a0498e-e431-4dc9-af94-a3e2a95cdad9.png">

(a) VDN (b) QMIX

Qtot에 대한 학습된 값을 보여주는 위의 표는 QMIX의 더 높은 표현 용량이 joint-action value 함수를 정확하게 나타낼 수 있는 반면 VDN은 그렇지 않다는 것을 보여줌



## 6. Experimental Setup

### 6.1. Decentralised StarCraft II Micromanagement

스타크래프트는 경쟁적이고 협력적인 멀티 에이전트 문제를 해결할 수 있는 좋은 기회를 제공-> StarCraft의 유닛에는 협업 에이전트 간의 복잡한 상호 작용을 학습할 수 있는 다양한 복잡한 micro-action이 있음

각 학습 에이전트가 개별 육군 부대를 제어하는 StarCraft II의 decentralised nicromanagement 문제에 집중-> 동일한 유닛의 두 그룹이 지도에서 대칭적으로 배치되는 전투 시나리오를 고려함-> 첫 번째 연합 그룹의 단위는 decentralised 에이전트에 의해 제어되고, 적 유닛은 heuristics를 사용하는 스타크래프트 II AI에 의해 제어됨

에이전트는 북쪽, 남쪽, 동쪽 또는 서쪽의 네 가지 방향으로만 이동가능, 적군이 사격장 내에 있을 경우에만 유닛이 공격[적]을 수행-> 이렇게 하면 문제를 분산시킬 수 있으며 게임에 통합된 공격 이동 매크로 액션을 사용할 수 없음, 또한 휴식 상태일 때 적의 발포에 대응하고 사정거리에 있는 적을 공격하는 장치 동작을 비활성화-> 에이전트들이 내장된 스타크래프트 II 유틸리티에 의존하지 않고 스스로 최적의 전투 전략을 찾기 위해 탐색하도록 강요



### 6.2. Ablations

mixing network 에서 추가 상태 정보 포함의 영향과 비선형 변환의 필요성을 조사하기 위해 절제 실험을 수행

- 첫째, hyper network가 없는 QMIX와 비교하여 mixing network에서 추가 상태 정보의 중요성을 분석 (QMIX-NS)
- 둘째, mixing network의 hidden layer을 제거하여 비선형 혼합의 필요성을 조사(QMIX-Lin)
- 셋째, 비선형 혼합과 비교하여 state를 활용하는 것의 중요성을 조사, 이를 위해 에이전트의 Q-Values 합계에 state-dependent term를 추가하여 VDN을 확장(VDN-S)
- 또한 각 에이전트가 가장 가까운 적을 공격하고 죽을 때까지 동일한 대상을 계속 공격하는 완전한 관찰 가능성을 가진 비학습 휴리스틱 기반 알고리즘의 성능을 보여줌-> 그 후, 에이전트는 가장 가까운 적을 공격



## 7. Results

방법의 각 실행마다 100회마다 훈련을 일시 중지하고 각 에이전트가 greedy decentralised action 선택을 수행하는 동안 20회의 독립 에피소드를 실행

제한 시간 내에 방법이 모든 적 유닛을 물리치는 이러한 에피소드의 백분율을 테스트 승률이라고 합니다.

<img width="521" alt="image" src="https://user-images.githubusercontent.com/60170358/170719123-7ec5306e-5913-4bae-9793-0f6c00f033bd.png">

-> 각 방법에 대한 20회 실행에서의 평균 테스트 승률을 95% 신뢰 구간과 함께 표시



### 7.1. Main Results

모든 시나리오에서 IQL은 적을 지속적으로 패배시키는 정책을 학습하지 못함

joint action-value 함수를 학습하는 것의 이점은 모든 시나리오에서 VDN이 IQL보다 우수한 성능을 발휘하는 것으로 입증될 수 있음

QMIX는 이종 에이전트 유형이 있는 맵에서 모든 맵에서 가장 강력한 성능을 발휘

### 7.2. Ablation Results

QMIX가 모든 Ablation을 수행했들때 우수함

<img width="507" alt="image" src="https://user-images.githubusercontent.com/60170358/170721609-c5f005b3-7f75-45c7-92ff-62328baabee7.png">

QMIX-NS는 두 시나리오 모두에서 VDN과 동등하거나 약간 더 나은 성능을 발휘하는데, 이는 복잡한 시나리오에서 cental state의 조건으로 하지 않을 때 비선형 분해가 항상 유익한 것은 아니라는 것을 시사

또한 QMIX-Lin과 비교하여 VDN-S의 성능은 central state를 완전히 활용하기 위해 비선형 혼합을 허용할 필요성을 보여줌

### 7.3. Learned Policies

다양한 방법으로 학습된 전략 간의 차이를 더 잘 이해하기 위해 정책의 학습된 행동을 조사

8m 시나리오에서 QMIX와 VDN은 측면으로부터 들어오는 적 부대를 향해 발사하기 위해 먼저 유닛을 반원형으로 배치하는 특히 정교한 전략을 학습

 2s 3z 시나리오에서, VDN은 먼저 왼쪽으로 실행한 다음, 위치 결정이나 유닛 매칭과 무관하게 일단 그들이 사정권에 들어오면 공격, 반면 QMIX는 적 광신도들이 스토커를 공격할 수 없도록 스토커를 배치하는 방법을 배움. QMIX의 3s 5z 시나리오에서도 동일한 행동이 관찰됨

VDN-S는 광신도로부터 스토커를 보호하는 방법을 배우지 않고, 먼저 시작 위치 주변에 유닛을 배치한 다음, 그들이 이동하면서 적을 공격

VDN과 IQL의 성능에서 초기 상승은 두 방법 모두 처음에 첫 번째 가시적 적을 공격하는 간단한 전략을 학습하기 때문->탐색적 학습 행동 때문에 (단순히 쏘는 대신) 이동하려고 시도하기도 하며, 이로 인해 성능이 급격히 저하

IQL은 초기 전략을 복구할 수 없는 반면, VDN은 작은 움직임과 발사를 함께 결합하는 방법을 학습



## 8. Conclusion

본 논문에서는 centralised setting에서 decentralised 정책의 end-to-end 학습을 가능하게 하고 추가 상태 정보를 효율적으로 사용하는 심층 다중 에이전트 RL 방법인 QMIX를 제시

QMIX는 에이전트별 action-value 함수로 다루기 쉬운 분해를 허용하는 풍부한 joint action value 함수의 학습을 허용-> 이는 혼합 네트워크에 단조성 제약 조건을 부과함으로써 달성

StarCraft II의 decentralised unit micromanagement task에 대한 결과는 QMIX가 independent Q-learning 뿐만 아니라 덜 정교한 joint state-value 함수 factorisation기능을 사용하는 다른 가치 기반 다중 에이전트 방법보다 최종 성능을 향상시킨다는 것을 보여줌

QMIX를 많은 학습 에이전트와의 설정에 대한 더욱 공동체의 탐색 체계로 보완하는 것을 목표로 함