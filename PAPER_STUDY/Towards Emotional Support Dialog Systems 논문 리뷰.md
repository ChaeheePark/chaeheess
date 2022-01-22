# Towards Emotional Support Dialog Systems 논문 리뷰

## Abstract

정서적 지원 (Emotional support)은 많은 대화에서 중요한 능력 -> 대화에서 정서적 지원을 구축하는 연구가 없음

이 논문에서는 많은 주석을 사용한 ESConv dataset (지원자-도움요청자) 와 풍부한 Annotation 구축함

효과적인 정서적 지원을 제공하는 데의 전략의 중요성과 더 많은 정서적 지원 시스템을 훈련하는것의 ESConv의 유용성을 보여줌



## Introduction

정서적 지원은 개인의 고통을 줄여주고 그들이 처한 문제를 해결하고 이해하는데 목표가 있음

사용자와 매일 상호작용 하는 대화시스템으로 학습시키는것은 매우 중요함(사회적 상호작용, 정신 건강 지원, 고객 서비스 채팅 등)

최근의 연구는 사람들이 더 많은 지원 응답을 제공하는 대화를 선호한다는것을 보여줌

그냥 정서적 지원을 제공하는것이 직관적이지 않은 연구결과로 인해, 대화를 통해 더 나은 지원을 제공하기 위해 절차와 대화 기술이 제안됨

<img width="239" alt="image" src="https://user-images.githubusercontent.com/60170358/150629309-8e440706-345d-457e-814e-9e3db8372228.png">

-> 지원자가 도움을 필요로 하는사람에게 제공하는 효과적인 (ESConv에서 채택한) 정서적 지원 을 보여주는 예시, 지원자가 사용한 지원전략 스킬은 괄호안에 표시됨, 빨간색 굵은 텍스트는 ESC framework의 세 단계                                                                                                                      

지원자는 도움 요청자의 고통의 원인을 파악하기 위해 문제를 탐색 -> 이 탐색과정이 없으면 지원자는 도움 요청자의 경험과 감정을 이해할 가능성이 낮아서 부적절한 조언을 하면 불쾌하거나 해로움

지원자는 도움 요청자의 상황을 알게되는 과정에서 다양한 스킬을(자기소개, 감정반영) 활용해서 이해와 공감을 표현해서 도움 요청자의 좌절감을 해소

지원자는 문제를 이해한 후 도움이 되는 제안-> 지원자가 변화할 행동에 대한 어떠한 영감 없이 위로만 한다면 지원자는 도움요청자의 정서 개선에 효과적으로 도움을 주지 못함

이 예제 대화의 데이터를 수집하는 동안, 도움 요청자는 정서 감도가 5에서 2로 감소했다고 보고함 (Emotional support의 효과)

ES의 중요성과 복잡성에도 불구하고, 대화의 관련 말뭉치의 부족으로 연구는 제한적
<img width="192" alt="image" src="https://user-images.githubusercontent.com/60170358/150629842-26b202c5-3869-4841-92d3-77bb379044fb.png">



1. emotional chatting 또는 empathetic responding은 기능성이 제한된 메세지를 반환
2. 사람들은 선천적으로 지지하는 것에 능숙하지 않기 때문에, 어떻게 더 지지적인 사람을 학습시키는지에 대한 가이드라인이 개발됨-> 그러한 가이드라인을 사용하는 말뭉치의 data-driven model은 지원 스킬을 명시적으로 학습해서 효과적인 ES를 제공할 수 있는 능력이 제한

본 논문에서는 전문적인 상담보다는 사회적 상호작용을 통해 지원을 제공하는 것을 목표로 ESC과제를 정의함

Hill's Helping Skills model of conversational suport 의 관련 구성 요소를 적용하여 대화 시스템을 위한 ESC framework를 설계 -> 탐색, 위로, 행동 세가지 단계

정서 지원 대화에 대한 연구를 용이하기 위해, ESConv dataset을 구성하고 많은 주석을 보장하기 위해 여러 노력을 기울임

ESConv는 도움 요청자와 지원자 역할을 하면서 채팅함

우리는 최첨단 모델을 평가하고 다양한 지원 전략이 활용되어질 때 정서 지원의 상당한 개선을 관찰함



## Related Work

### 1. Emotional & Empathetic Conversation

감정은 매력적인 대화 시스템을 구축하는데 중요함

Zhou et al. (2018)는 ECM을 제안해서 미리 지정된 감정을 바탕으로 감정 반응을 이끌어 내는 연구를 제안-> 이 작업은 생성된 응답에서 감정을 정확하게 표현하기 위해 필요, ES는 감정을 표현할 수 있지만, 감정 채팅과는 달리 사용자의 정서적 고통을 줄이려는 광범위한 목적을 가지는데 목적을 둬야함 (ES가 더 수준 높고 복잡한 능력)

또다른 관련 tastk는 Rashkin et al. (2019)은 대화 모델은 대화자의 감정을 인식함으로써 더 많은 공감 반응을 생성한다고 주장-> 효과적인 ES는 자연스럽게 도움 요청자의 경험과 느낌에 공감을 표현해함 (공감은 정서적 지원의 필수적 구성요소)

### 2. Related Datasets for Emotional Support

다양한 연구는 소셜미디어나 온라인 포럼과 같은 사회적 맥락에서 정서적 지원의 대화를 고려함

트위터에서 스트레스 관련 게시물과 응답 쌍을 수집하고 답변을 지원 카테고리로 분류

TalkLife와 정신 건강 하위레딧의 응답쌍은 텍스트 기반 공감 표현의 의사소통으로 주석을 달음

이러한 말뭉치의 대화는 단 방향 상호작용 또는 매우 짧은 대화로 ES 잠재능력을 제한함

### 3. Emotional Support Dialog Systems

일부 전통적인 대화 시스템은 정서적 지원 응답을 제공하기 위해 인간이 만든 규칙을 적용

최근 시스템은 규칙 기반 알고리즘을 사용했음 (응답으로 지원 행동을 결정하고 사전 정의된 후보 목록에서 적절한 응답을 선택)

사용자가 언급한 주제를 식별한 다음 미리 정의된 사전 메시지로 대응함으로써 COVID-19에 대처하기 위한 지원을 제공하도록 설계된 대화 시스템을 구현

지원 반응을 생성하는데 초점을 둔 연구는 거의 없고 범위가 제한적임



## Emotional Support Conversation

### 1. Task Definition

사용자가 나쁜 감정 상태에 있을때, 사용자들은 감정 상태를 개선하기 위해 도움을 구할 수 있음

사용자는 부정적인 감정(1-5)과 사용자가 껶고있는 어려움으로 태그 됨

지원자는 지원 스킬로 대화할 때 사용자를 위로하여 강도 수준을 낮춰야 함

대화하는 동안 지지자는 사용자의 문제를 파악하여 위로한 후 사용자가 자신의 문제에 대처할 수 있도록 몇가지 제안이나 정보를 제공해야함

정서적 지원 대화는 대화 마지막에 사용자의 강도가 낮아지거나 좀더 구체적으로는 지원자가 문제를 효과적으로 파악하여 사용자를 위로하고 해결책이나 제안을 제공한다면 효과적임

ESC과제는 세가지가 있음

1. 지원 전략 선택과 전략 제한 응답 생성 (전략을 사용하는 타이밍은 ES효과와 관련이 있음-> 생성된 반응이 지정된 전략을 따르는 것이 중요)
2. 감정 상태 모델링 (동적 전략 선택과 ESC 효과 측정을 위해 사용자의 감정 상태를 모델링하고 추적하는 것이 중요)
3. 지원 효과성 평가 (대화의 관련성, 일관성, 사용자 참여를 평가하는 전통적인 차원과 더불어 ESC는 새로운 차원을 제시)

### 2. ESC Framework

정서적 지원 절차를 세 단계로 하는 ESC framework (단순한 전문적인 상담이 아닌 사회적 상호작용을 통해 지원을 제공)

<img width="480" alt="image" src="https://user-images.githubusercontent.com/60170358/150631932-77e10f24-087f-4cfe-9654-c1568d230a74.png">

탐색 -> 위로 -> 동작

Lexical Feature열은 데이터셋의 각 전략을 사용하는 메세지와 관련된 상위 5개의 유니그램 또는 빅그램을 표시함 (반올림된 z점수 로그 odds 비율로 순위가 매겨짐)

**Stages**

탐색 (도움 요청자가 문제를 확인할 수 있도록 탐색), 통찰 (도움 요청자가 자기 이해의 새로운 깊이까지 이동할 수 있게 도움), 행동(도움 요청자가 문제를 해결하는 행동을 결정할 수 있게 도움)

그러나 통찰력에는 사용자의 행동과 감정을 다시 해석해야 하는데 이는 충분한 지원경험이 없는 지원자에게는 어렵고 위험함 -> 따라서 위로에 대한 통찰력을 적응시킴 (공감과 이해를 통해 지원을 제공)

하지만, 실제로는 대화는 고정된 순서와 선형 순서를 따를 수 없어 유연하게 대응해야함

**Strategies**

Hill은 또한 각 단계에 대해 몇가지 권장 대화 기술을 제공함

대화 스킬이 어떠한 전문적인 감독이 없는 상태에서는 적절하지 않을 수 있음 -> 그래서 7가지 방법을 제안함



## Data Collection

ESConv dataset을 사용

1) 대화 지원을 제공하는 것은 지지자가 효과적으로 되기 위해 학습되어야 하는 기술이기 때문에 ESC framework로 튜토리얼을 설계하고 지지자가 되도록 crowdworker을 훈련시킴 -> 시험에 합격한 crowdworker만 작업에 참여 할 수 있음
2) 도움 요청자들이 그들의 문제와 감정에 대한 사전 채팅 조사를 완료하고 대화 도중과 대화 후에 피드백을 제공하도록 요청
3) 원래의 대화 데이터를 수집한 후에 낮은 질의 대화를 걸러내기 위해 자동 메커니즘을 고안

### 1. Supporter-specific Tasks

**Training and Examination**

crowdworkers에게 효과적인 정서적 지원을 제공하는 방법을 가르치기 위해 ESC framework로 튜토리얼을 설계: 7개 단계에서 영감을 받아 11개 하위 단계를 개발하여 crowdworker 가 학습할 수 있도록 함, 사용자가 고정된 순서를 따르는 것이 불가능할 수 있어 융통성이 필요 할 수 있음을 알림

**Strategy Annotation**

지원자들이 대화중에 ESC 지원 전략을 사용하도록 하고 결과 데이터셋을 구축하기 위해, 지원자들이 먼저 적절한 전략을 선택하도록 함

지원자들이 여러 전략을 사용해서 지원을 원하는 경우 많은 메세지를 보낼것을 독려함

**Post-chat Survey**

각각 대화가 끝난 후 지원자는 정도를 평가받도록 요청됨 -> 도움 요청자가 그들의 문제에 대해서 상세히 설명하는 정도를 5점 만점의 Likert scale로 

 ### 2. Seeker-specific Tasks

**Pre-chat Survey**

각 대화 전에, 도움 요청자는 다음과 같은 survey를 수행하도록 요청됨

1) Problem & emotion category (5개의 옵션에서 문제하나 선택, 7개의 옵션에서 감정하나 선택)
2) Emotion intensity (1-5)
3) Situation
4) Experience origin (이전 삶의 상황에서 비롯된 것인지 현재 경험인지)

**Feedback**

대화 도중, 도움 요청자는 지원자로 부터 받은 두번에 새로운 발언 후에 피드백을 해달라는 요청을 받음

그들의 피드백은 별 5개 척도의 지원 메시지의 유용성을 채점함

각 대화를 3단계로 나누고 평균 피드백 점수를 계산함 (4.03 / 4.30 / 4.44 로 효과적으로 도움을 받음을 알 수 있음)

**Post-chat Survey**

각 대화가 끝난 후, 도움 요청자는 5점 기준으로 그들의 감정과 지원자의 행동을 Likert scale로 평가하도록 요청받음

1. 감정 지원 대화 후의 그들의 감정 강도
2. 도움 요청자의 경험과 느낌의 지원자의 공감과 이해
3. 대화 주제에 대한 지원자의 반응의 관련성

### 3. Quality Control

corpus가 효과적인 감정 지원 대화의 고품질의 예시를 포함하도록 보장하기 위해 여러 방법을 사용

**Preliminary Filtering Mechanisms**

지원자의 참가자를 모집했고 필터링을 통해 1342건의 대화를 남김

**Auto-approval Program for Qualified Conversations**

데이터 품질 관리에서 가장 중요한 부분인 auto-approval program을 세심하게 설계함

<img width="225" alt="image" src="https://user-images.githubusercontent.com/60170358/150633289-9964e262-1f2a-429b-91e9-8cd44f9240c9.png">

대화 전 평균 감정 감도는 4.04 이고 그 후는 2.14 : 정서적 지원의 효과를 보여줌

대화 후 감정감도에 설문조사가 없는 경우는 (2), (3)의 요건만 충족한 경우만 데이터를 남김-> 1053개가 남음

**Annotation Correction**

지원 전략과 도움 요청자의 감정 강도에 대한 잘못된 주석을 검토하고 수정함

1. 전략 수정을 위해, 새로운 자격을 갖춘 지원자들에게 주석을 검토하고 수정할 것을 요청하고 139개를 수정
2. 자동 승인 기준에 따르면 정서 개선 점수가 1점 미만 일때 자격이 주어지지만 3개의 기준은 충족됨(?) -> 부정적인 감정강도를 긍정적인 감정강도 오해한 사람들이 대부분 이런 결과라서 수정



## Data Characteristics

### 1. Statistics

<img width="221" alt="image" src="https://user-images.githubusercontent.com/60170358/150633565-50ec1412-fc04-4208-aa8a-59010c66aae5.png">

1053개의 ESConv 예제의 통계는 위의 표에 나와있음

상대적으로 긴 대화(29.8)는 효과적인 ES를 제공하기 위해 이전의 감정 채팅 또는 공감 대화 데이터 셋에 비해 많은 상호작용 과턴이 필요함

<img width="225" alt="image" src="https://user-images.githubusercontent.com/60170358/150633661-41ca6588-5f02-476d-8a70-ec539a991900.png">

주석의 통계는 위의 표에 나와있음

COVID-19로 인해 지속적인 우울증과 취업 위기가 도움 요청자에게 가장 흔하게 언급되는 문제이고 우울증과 불안은 흔하게 언급되는 감정임

도움 요청자들의 피드백에서 정서적 지원을 매우 만족한다는 것을 발견함-> ESC framework에 기반한 훈련 튜토리얼이 지원자들이 효과적인 ES 지원을 배우는데 도움을 줌

### 2. Strategy Analysis

**Lexical Features**

다른 모든 전략과 대조되는 각각의 전략에 대한 유니그램과 빅그램의 log odds 비율, 유용한 Dirichlet prior을 계산함으로써 각각의 전략의 Lexical Feature을 추출함

**Strategy Distribution**

대화의 여러단계에서 전략의 분포를 계산

전체 L개의 발화를 가진 대화에서, k번째의 발화는 지원자로부터 나왔고 st 전략을 채택 (k/L 위치에 있음)

구체적으로 대화 진행을 6개 구간으로 나눔

<img width="234" alt="image" src="https://user-images.githubusercontent.com/60170358/150639724-06da65e9-1edb-44a5-9a08-4a415d7905f9.png">

그 6개의 비율을 각각 그리고 연결해서위와 같은 표를 얻음

지원자들은 일반적으로 ESC framework가 제시한 stage 순서를 따르지만 유연한 조정도 필요

예를들어 대화의 초기단계에서 지원자들은 질문과 같은 탐색 전략을 채택하고, 그 뒤에 제안을 제공함

전체 대화에서 위로 략을 사용하고 비교적 일정한 비율의 메세지에 레이블을 붙임

**Strategy Transition**

3/4 홉으로 가장 빈번한 전략 전환을 제시

이러한 전환은 ESC framework 훈련의 튜토리얼로, 지원자들은 보통 질문하고 도움 요청자들을 위로하기 전에 상황을 탐색하는 것을 나타냄



## Experiments

1. 전략 주석이 있는 ESConv가 최첨단 generative 대화 모델을 얼마나 개선할 수 있을까?
2. 이 모델들이 ESConv로 부터 효과적인 정서적 지원을 제공하는 것을 배울 수 있을까?

### 1. Backbone Models

2개의 최첨단 pre-trained model을 사용

**BlenderBot**

공감 응답을 포함한 많은 대화 스킬로 학습된 open-domain conversational agent 

사용자를 위한 ES를 제공할 수 있어야 함

**DialoGPT**

대규모 대화 말뭉치에 pre-trained GPT-2 기반 모델을 추가로 사용

### 2. Variant Models

**Vanilla**

전략 주석에 엑세스 하지 않고 ESConv 에서 backbone 모델을 직접 미세조정

**Variants with strategy**

전략 주석을 backbone 모델에 통합하기 위해 각 전략을 나타내는 특별한 token 사용 , 지원자들의 각 발화 y에 [st] token을 덧붙임

그 다음 flatten dialog 기록 x를 입력으로 사용해서 모델은 첫번째 예측 전략 token에서 조건화 된 반응을 생성

이후 전략 주석을 사용하는 세가지 변형을 연구함

1. Oracle: 응답이 gold 참조 전략 token에 따라 생성
2. Joint: 응답이 예측 전략 토큰에 따라 생성
3. 무작위: 무작위로 선택된 전략에 따라 생성

### 3. Automatic Evaluation

BlenderBot or DialoGPT를 backbone으로 하여 지원 전략 활용이 모델 성능에 미치는 영향을 조사하기 위해 variants의 성능을 비교함

<img width="223" alt="image" src="https://user-images.githubusercontent.com/60170358/150640595-9630f4c9-0807-4e01-bb10-01dab0994be7.png">

세가지 주요 결과가 있음

1. Oracle 모델은 모든 메트릭에서 Vanilla 모델보다 월등히 우수하여 지원 전략의 큰 효용성을 나타냄
2. Joint model은 Vanilla 모델보다 훨씬 낮은 점수를 얻음
3. BlenderBot 변형이 DialoGPT 변형보다 더 나은 성능을 발휘해서 ESC task에 더 적합하다는 것을 나타냄

### 4. Human Interactive Evaluation

모델들과 채팅을 하기 위해 참가자를 모집

온라인 테스트는 모델은 지원자의 역할을 수행, 무작위로 순서가 주어진 두 개의 다른 모델과 채팅함

참가자들은 질문에 근거하여 두 모델을 비교하도록 요청받음(2,3,4는 ESC framework의 3단계에 해당)

1. 유창함
2. 식별: 상황을 깊이 탐색하고 문제를 잘 파악하는지
3. 위로
4. 제안
5. 전반적

3쌍의 모델을 비교함 (사람과 100가지 대화로)

a. Joint vs Blenderbot ->fine-tuning 후 BlenderBot 기능이 크게 향상

b. Joint vs Vanilla -> 전략을 사용하는 것이 더 큰 comfort를 줄 수 있음

c. Joint vs Random (전략 분포에 따라 전략을 무작위로 선택) -> 전략의 적절한 타이밍이 사용자에게 문제를 식별하고 효과적인 제안을 제공하는데 중요한 역할을 함

<img width="217" alt="image" src="https://user-images.githubusercontent.com/60170358/150640939-a97ea9af-b048-49e8-a8aa-0635a5a0f68f.png">

일반적으로 ESConv에 대한 전략 예측 감독과 함께 fine-tuned 됨으로써 ESConv의 높은 품질과 효용성을 증명하는 pre-trained model이 선호됨

### 5. Further Analysis of Human Interactive Evaluation

ESConv에서 dialog model이 학습한 내용을 살펴봄

인간 상호 작용 실험에서 사용자와 Joint model 사이에 300개의 대화를 기반으로 전략 분포를 분석

<img width="228" alt="image" src="https://user-images.githubusercontent.com/60170358/150641096-84cd3b6c-5535-4027-a463-42e380f329ba.png">

Joint model이 채택한 전략은 ESConv의 분포와 매우 유사함-> 모델이 효과적인 ES를 달성하기 위해 인간 지원자들이 하는 것처럼 전략 선택과 활용을 모방함

Joint model은 더 도움이 되는 응답을 제공하고 많은 기술을 사용하는 반면 fine-tuning이 없는 Blenderbot은 사용자의 고통을 잘 이해하지 못하고 자신에 대해 더 많이 말하기를 선오함

-> 더 많은 지원 반응과 다양한 지원 전략을 갖는 것이 효과적인 정서적 지원에 중요함



## Conclusion

정서적 지원 대화의 과제를 정의하고 ESC framework를 제시함

ESC framework는 Helping Skills 이론에 각 단계에서 유용한 지원 저냑으로 3단계를 특징짓는 대화 시스템 설정으로 채택 -> ESConv dataset 구성 ->데이터 수집 프로세스를 신중하게 설계하고 ES의 효과를 보장하기 위해 여러 매커니즘을 고안->최첨단 대화 상자 모델을 사용하여 ES 능력 평가

본 연구는 정서적 지원이 중요한 다른 대화 시나리오에 대한 모델을 개선할 수 있음