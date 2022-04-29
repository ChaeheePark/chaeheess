# Emotion Recognition in Conversation: Research Challenges, Datasets, and Recent Advances 논문 리뷰

## Abstrct

Emotion recognition in conversation (ERC) - 대화에서의 감정인식: 사용자의 감정을 이해해야 하는 감정 인식 대화를 생성하는 데에도 매우 중요해서 효과적이고 확장 가능한 대화 감정 인식 알고리즘이 필요



## Introduction

the presence of contextual cues, the temporality in speakers’ turns, or speaker-specific information(맥락적 단서의 존재, 화자의 차례의 시간성 또는 화자 특정 정보)이 ERC에서 중요

<img width="530" alt="image" src="https://user-images.githubusercontent.com/60170358/162694576-05f52e3b-9aa7-413c-9b4a-db0022a4ebd6.png">

(같은 Yeah도 다른 느낌)



### Task Definition

미리 정의된 여러 감정으로부터 각 발화의 감정을 식별하는 것을 목표

<img width="263" alt="image" src="https://user-images.githubusercontent.com/60170358/162696064-853fdd5f-36ad-4731-ba71-966ef95e6218.png">

### Controlling variables in conversations

task-oriented or non-task oriented(채팅) -> 주제, 대화자의 성격, 논증 논리, 관점, 의도에 의해 통제

<img width="272" alt="image" src="https://user-images.githubusercontent.com/60170358/162696567-757e2c26-389c-4776-88e0-96e20691b7d8.png">

**이전 발화**는 과제 지향 대화 또는 화자의 감정 상태(채팅)에서 가장 큰 변화를 주기 때문에 가장 중요한 발화

화자의 상태+화자의 의도+감정+이전발언 = 현재 발언

<img width="350" alt="image" src="https://user-images.githubusercontent.com/60170358/165772801-3a2f7c97-e55e-43d9-b2a6-3695f91a623d.png">

human과 medical chatbot 사이의 conversation : chatbot은 human의 감정에 기반하여 반응

**본 논문에서는 대화에서 감정 역학에 기여하는 다양한 요소에 대해 논의**



## Research Challenges

ERC의 최근 work인 DialogRNN or ICCON은 ERC 문제를 어렵게 만드는 요인을 설명

**a) categorization of emotions**: emotion은 2개의 type-> categorical and dimensional

- categorical: emotion을 discrete categories의 고정된 number로 나눔<img width="314" alt="image" src="https://user-images.githubusercontent.com/60170358/165884351-7069b1b3-0979-4056-9309-17fa0d2906b1.png">

  

  Plutchick의 emotion/Ekman은 6개로 나눔

- dimensional: 연속적 multi-dimensional space로 emotion을 설명->2개의 dimension valence/arousal

  valence: positivity /arousal: intensity

  감정을 딱딱한 범주가 아닌 연속적인 스펙트럼으로 매핑-> categorical에 비해 벡터 연산을 사용하여 두 감정 상태를 쉽고 직관적으로 비교가능

  Ekman의 모델과 같은 간단한 분류 모델을 선택하는 것은 이러한 모델들이 복잡한 감정을 접지할 수 없기 때문고, 플루치크의 모델과 같은 복잡한 감정 모델은 주석자들이 관련 감정을 구별하는 것을 매우 어렵게 만듬

  인기 있는 ERC 데이터 세트 IEMOCAP은 범주형 모델과 차원 모델을 모두 채택,DailyDialog와 같은 새로운 ERC 데이터 세트는 보다 직관적인 특성 때문에 범주형 모델만 채택
  EmoContext 데이터 세트의 각 감정 발화는 행복, 슬픔, 분노 중 하나의 감정으로 분류



**b) Basis of emotion annotation**: 레이블은 주석자의 관점에 따라 달라지기 때문에 감정 레이블로 주석을 다는 것은 어려움, 주석자는 상황 인식 주석을 위해 대화자의 관점을 인식해야함

**c) Conversational context modeling**: context는 NLP 연구의 핵심-> contextual 문장과 단어 임베딩은 최첨단 NLP 시스템의 성능을 크게 향상시킴
문맥의 개념은 문제마다 다를 수 있음-> 주변의 발언들은 문맥으로 취급되고 그것들은 문맥적 증거가 실제로 분류에 도움이 됨, 마찬가지로 대화 감정 감지에서도 시간 t에서 발화의 감정을 결정하기 위해 시간 <t에서 앞서 말한 것을 문맥으로 간주할 수 있음

그러나 이러한 문맥 표현을 계산하는 것은 감정 역학(emotional dynamics)으로 인해 어려움을 겪음->대화의 정서적 역학은 자아 의존성, 대인 의존성으로 나뉨
자아 의존성: 대화하는 동안 화자가 자신에게 미치는 정서적 영향

대인 의존성: 상대방이 화자에게 유도하는 정서적 영향

반대로, 대화하는 동안, 화자들은 상대방을 거울삼아 친밀감을 쌓는 경향도 있음

<img width="340" alt="image" src="https://user-images.githubusercontent.com/60170358/165891523-6caf56fb-056a-4692-81ee-52e254064a51.png">

Pa는 장기 실업에 좌절하고 격려를 구하지만 Pb는 비꼬는 투로 대답->Pa가 분노의 반응(u6)을 적합하게 하도록 격분시킴

Pb는 정서적으로 영향을 받지 않지만 Pa는 Pb에게 정서적으로 영향을 받음

자아와 대인 관계와 의존성을 모델링하는 것은 또한 대화의 주제뿐만 아니라 논쟁 구조, 대화자의 성격, 의도, 대화에 대한 관점, 서로에 대한 태도 등과 같은 다양한 다른 요소들에 따라 달라질 수 있음: 이러한 모든 요소를 분석하는 것은 풍부한 상황 이해로 이어질 수 있는 진정한 자아 및 대인 관계 의존성 모델링의 핵심

상황별 정보는 local 및 원격 대화 기록에서 모두 얻을 수 있고 로컬 컨텍스트는 중요하지만 원격대화기록은 덜중요-> 이전의 발언을 얘기할때 중요할 수 있음

문맥의 유용성은 대화의 맥락과 담론에 따라 다른 감정을 표현할 수 있는 '예', '오케이', '아니오'와 같은 짧은 발화들을 분류할 때 더 우세함

<img width="643" alt="image" src="https://user-images.githubusercontent.com/60170358/165892542-577161d9-1b04-40b8-9c28-cad76a0dcec0.png"> 이 두 가지 예에서 같은 "Yeah"로 표현되는 감정은 서로 다르며 문맥에서만 추론할 수 있음

문맥에 맞는 대화 발화 표현을 찾는 것은 활발한 연구 분야이고 상황별 단서를 활용하는 것은 어려운 작업임

**d) Speaker specific modeling**: 개인들은 감정을 표현하는 그들만의 미묘한 방법을 가지고 있음

예를 들어, Pa → "주문이 취소되었습니다!"  Pb → "이것 참 잘됐군요!" ->만약 Pb가 비꼬는 사람이라면, 그의 반응은 great라는 단어를 통해 주문이 취소되는 것에 대해 부정적인 감정을 나타내는 반면, 취소된 주문이 Pb에게 유리하다면 Pb의 반응은 문자 그대로 받아들여질 수 있음

이전 발언을 바탕으로 한 화자 프로파일링은 종종 개선된 결과를 산출함

**e) Listener specific modeling**: 듣는 사람들은 말하는 사람의 말에 대해 마음을 정함, 그러나 듣는 사람이 말하는 사람에 대한 반응에 대한 텍스트 데이터가 없음

모델은 청취자의 반응을 포착하기 위해 청취자의 얼굴 표정을 모델링하기 위해 시각적 모달리티에 의존해야 함

그러나 DialogueRNN에 따르면 청취자의 반응을 포착하는 것은 청취자의 후속 발화가 반응을 전달하기 때문에 개선이 없음

하지만 각 발화의 감정 인식과 반대되는 정치적 연설 중 청중의 반응과 같이 대화의 매 순간의 지속적인 감정 인식이 필요한 시나리오에서 유용할 수 있음

**g) Fine-grained emotion recognition**:  세밀한 감정 인식은 명시적이고 암묵적인 주제에 표현된 감정을 인식하는 것을 목표로 함=대화의 주제, 대화자의 의견과 입장을 더 깊이 이해

<img width="165" alt="image" src="https://user-images.githubusercontent.com/60170358/165893513-a40f88e4-8275-407f-90de-476c7984af73.png">

예를들어, 두 사람 모두 정부 법안에 대해 지지 입장을 취하고 있지만, 그들은 그것을 표현하기 위해 완전히 반대되는 감정을 사용 -> 바닐라 감정 인식기가 정부 법안의 측면에 대한 두 대화자의 긍정적인 감정을 이해하는 것은 가능하지 않음

**h) Multiparty conversation**:  다자간 대화에서 감정 인식은 개별 화자 상태를 추적하고 공동 참조를 처리하는 데 어려움이 있기 때문에 이중 대화와 비교하여 더 어려움

**i) Presence of sarcasm**: ''빈정거림''은 경멸을 표현하기 위해 아이러니를 사용하는 언어적 도구 -> 대화에서 빈정거림을 감지하는 것은 대화의 맥락과 담론에 크게 좌우됨

**j) Emotion reasoning**: 추론 능력은 설명 가능한 모든 AI 시스템에 필요함-> ERC의 맥락에서, 화자가 표현하는 감정의 원인을 이해하는 것이 바람직함

<img width="340" alt="image" src="https://user-images.githubusercontent.com/60170358/165891523-6caf56fb-056a-4692-81ee-52e254064a51.png">

그림 2에서, 감정 추론의 능력을 갖춘 이상적인 ERC 시스템은 u6에 표현된 Pa의 분노의 이유를 인식해야함

상황 모델링과 감정 추론을 혼동해서는 안됨: 상황 모델링과 달리, 감정 추론은 대화의 역사에서 발화의 감정을 촉발하는 맥락적 발화를 찾을 뿐만 아니라, 대상 발화에 대한 상황적 발화의 기능을 결정함

그림 2에서 u4와 u5가 비추는 Pb의 무관심이 Pa를 화나게 함

<img width="562" alt="image" src="https://user-images.githubusercontent.com/60170358/165894317-51145334-c4e6-422e-8166-c9b53ef7a9c6.png">

조이는 이전 발언에서 챈들러의 속임수를 확인하면 분노를 표출함

감정 추론을 위한 분류법이나 태그 집합을 정의하는 것은 어렵고 사용 가능한 데이터 세트는 없지만 데이터 세트를 구축하면 미래의 대화 시스템이 의미 있는 논증 논리 및 담론 구조를 프레임화하여 인간과 같은 대화에 한 걸음 더 다가갈 수 있음
