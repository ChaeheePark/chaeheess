# Text2Gestures: A Transformer-Based Network for Generating Emotive Body Gestures for Virtual Agents 논문 리뷰

## Abstract

**자연어 텍스트 입력과 정렬된 가상 에이전트에 대해 대화식으로 감정적인 full-body gesture을 생성하는 transformer 기반 학습 방법인 Text2Gestures**

감정적 특징이라고도 알려진 신체 표현에 관련된 생체역학적 특징을 활용하여 감정적으로 표현되는 제스처를 생성

텍스트와 target 가상 에이전트의 의도된 성별 및 손동작에 해당하는 의도된 작업을 generation pipeline에서 고려

MPI Effective Body Expressions Database에서 네트워크를 교육하고 평가하며, 네트워크가 나레이션 또는 대화를 텍스트로 정렬된 가상 에이전트의 제스처를 생성하는데 있어 최첨단 성능을 발휘하는 것을 관찰



## 1 Introduction

세계적으로 일상적인 커뮤니케이션 및 상호 작용을 위해 디지털 및 가상 플랫폼을 점점 더 많이 사용함에 따라, 사회적 및 정서적 지능을 갖춘 매우 현실적인 가상 에이전트를 만들어야 할 필요성이 높아짐

대화식 음성 정렬 제스처 생성을 위한 기존 접근 방식은 음성 신호와 생성된 제스처 시퀀스 사이의 매핑을 학습 ->이러한 음성 기반 방법과 달리, 이 논문에서는 제스처를 자연어 텍스트 스크립트에 직접 맞춤

텍스트에 맞게 정렬된 제스처를 생성하는 이전의 학습은 다양한 sequence to sequence 예측 작업을 수행하는 데 효율적인 잘 알려진 sequence-to-sequence modeling network를 활용

최근의 접근법은 애니메이션의 인지된 자연스러움을 개선하기 위해 관련 보행 기반 정서적 특징을 활용

제스처를 감정적으로 표현하기 위한 감정적인 특징을 사용 (인간 참가자가 자연어를 기반으로 한 가상 에이전트로부터 적절한 감정을 인지할 수 있는)

**자연어 텍스트에 맞춰 감정적인 몸짓을 만들어내는 end-to-end 학습 가능한 생성 네트워크를 제시**- 가상 에이전트가 대사를 설명하거나 대화에 참여하는 대화형 애플리케이션을 위한 방법을 설계=transformer network 사용, 현재 접근 방식을 확장하여 가상 에이전트에 대한 제스처를 3D로 제공, 가상 에이전트의 관련 제스처 기반 감정 기능을 활용하여 제스처를 감정적으로 표현

- 대화식으로 한 번에 한 문장씩 텍스트를 받아들이고 해당 텍스트에 정렬된 제스처에 해당하는 가상 에이전트의 3D 포즈 시퀀스를 생성하는 transformer-based network
- 나레이션 또는 대화라는 의도된 작업 및 가상 에이전트의 의도된 성별과 손짓을 따르도록 generation process를 조절
- 감정적으로 표현된 제스처를 만들기 위해 텍스트의 의도된 감정을 고려
- 총 600개의 응답으로 구성된 웹 연구로 모션 캡처된 시퀀스와 생성된 제스처의 감정적 표현성을 비교한 결과



## 2 Related Work

### 2.1 Perceiving Emotions from Body Expressions

심리학 연구는 몸짓을 포함한 신체 표현이 분노, 안도, 두려움, 그리고 자부심과 같이 흥분과 지배에 따라 변화하는 감정을 표현하고 인지하는 데 얼굴과 목소리 신호보다 더 적합하다는 것을 알려줌

->우리의 작업에서는 제스처에서 관찰할 수 있는 이러한 감성적 기능을 사용하여 생성된 가상 에이전트를 emote함



### 2.2 Generating Emotive Virtual Agents

가상 에이전트에 감정 표현을 부여하기 위한 현재의 접근 방식은 언어 커뮤니케이션, 얼굴 움직임, 몸짓 및 걸음걸이 포함한 여러 가지 방식을 사용

말과 일치하는 감정 표현을 생성하는 맥락에서, 감정 표정에 매핑된 단어 데이터 세트를 활용하여 기본적인 감정을 가진 가상 에이전트를 자동으로 생성하는 연구와 사전 구축된 정신 상태와 신체 표현 간의 매핑 말뭉치를 사용하여 가상 에이전트가 적절하게 표현하도록 하는 본격적인 가상 인간 상담사 개발 연구 존재

-> 우리는 텍스트 대본과 관련된 더 다양한 범위의 의도된 감정으로부터 신체 제스처에 대한 일반화 가능한 데이터 중심 매핑을 구축하여 데이터베이스 밖의 텍스트 문장에 대해 적절하게 표현적인 제스처를 생성할 수 있게함



### 2.3 Generating Gestures Aligned with Speech and Text

최근에는 음성 콘텐츠와 일치하는 제스처를 생성하는 데 대한 광범위한 딥러닝 기반 연구가 진행

음성과 제스처 사이의 latent 매핑을 배우기 위해 숨겨진 마르코프 모델을 사용

RNN을 사용하여 입력 음성에서 제스처를 위한 3D 포즈 시퀀스를 예측 

음성과 제스처 데이터 사이의 latent 표현을 학습하도록 자동 인코더를 훈련시킨 다음, 훈련 데이터의 노이즈에 덜 민감한 제스처를 생성하기 위해 둘 사이의 매핑을 학습

반대로, 음성과 제스처 공간 사이의 역하위 변환을 학습하여 음성에 대응하는 최적의 제스처 세트를 확률적으로 생성

다른 접근 방식도 제스처에 개별 스타일을 통합하고, 생성된 제스처를 보다 사실적으로 보이게 하기 위해 여러 적대적 손실을 추가했으며, 심지어 담화을 기반으로 한 고개를 끄덕이거나 손을 흔드는 등의 원형 규칙 기반 행동을 추가

-> 이러한 작업은 style-transfer을 통해 이야기하는 사람을 위한 제스처를 생성하고, 말에서 의미 인식 제스처를 생성하는 등의 작업을 함

-> 우리는 말의 텍스트 대본에서 제스처로의 매핑을 배운다는 점에서 이러한 접근 방식을 보완하여 음성 신호의 노이즈를 제거하고 관련 내용과 맥락에만 집중할 수 있도록 도와줌,  글로부터 배우는 것은 상징적, 신성함, 그리고 은유적인 제스처를 포함한 더 넓은 범위의 제스처에 집중할 수 있게 해줌

-> end-to-end의 학습가능한 transformer network를 사용하여 보다 인간과 유사한 가상 에이전트의 의미 인식, 전신 제스처에 해당하는 3D 포즈 시퀀스에 텍스트 대본을 매핑하는 방법을 배움



### 2.4 Generating Stylistic Human Body Motions

감정적 표현력을 가진 음성 또는 텍스트 정렬 제스처를 생성하는 것은 얼굴 움직임, 머리 움직임 및 이동을 포함한 스타일리시한 인체 동작을 생성하는 데 있어 하위 문제로 간주됨

얼굴 동작에 대한 기존의 접근법에는 RNN 또는 CNN을 사용하여 입술 움직임과 말로 정렬된 다른 얼굴-근육 움직임을 생성하는 것이 포함됨

말의 속도와 강도를 전달하는 헤드 모션을 생성하는 방법은 자동 인코더와 생성적 적대 네트워크를 기반으로 한 신경 네트워크 아키텍처를 탐색

스타일리시한 이동을 생성하는 방법은 CNN, parametric phase function, deep learned phase function을 기반으로 함

최근의 접근법은 정서적으로 표현적인 걷기를 생성하기 위해 보행 기반의 정서적 특징을 통합

더불어, 순간과 행동에 대한 텍스트 설명을 기반으로 신체 동작의 이미지와 동영상을 생성하는 데 상당한 진전이 있었음

-> 우리는 텍스트 문장에 해당하는 대화형 속도로 감정적으로 표현되는 제스처를 생성하는 것을 목표



## 3 TRANSFORMING TEXT TO GESTURES

### 3.1 Representing Text

input sentence W=[w1,w2,..,wtsen]

GloVe model을 이용해 word embedding 얻음

Word2Vec 및 FastText 와 같은 다른 유사한 차원 임베딩 모델보다 성능이 우수하고 BERT 와 같은 훨씬 더 높은 차원 임베딩 모델과 유사한 성능을 보인 예비 실험을 바탕으로 Glove를 선택

시작(SoS) 및 시퀀스 끝(EoS) 벡터를 사용하여 문장의 시작과 끝을 구분



### 3.2 Representing Gestures

<img width="274" alt="image" src="https://user-images.githubusercontent.com/60170358/160269990-e92fa666-ad58-47a2-bddb-b36112a6d74f.png">

각 포즈는 방향 포즈 그래프에서 부모에 대한 각 관절의 3D 회전을 나타내는 4분의 1로 나타냄

gesture Q=[q1,q2,...,qtges]

각 제스처 시퀀스의 시작과 끝을 구분하기 위해 시퀀스 시작(SoS) 및 시퀀스 종료(EoS) 포즈를 정의, 이 두 가지 모두 엔드 이펙터 관절, 뿌리, 손목 및 발가락의 위치가 변경되는 앉은 자세



### 3.3 Representing the Agent Attributes

attributes depending on the input text,  attributes depending on the virtual agent 2가지로 분류

#### 3.3.1 Attributes Depending on Text

**Acting Task**

나레이션: 에이전트는 이야기를 듣는 사람에게 대사를 내레이션, 이 경우 제스처는 일반적으로 더 과장되고 연극적

대화: 에이전트는 몸짓을 사용하여 다른 에이전트 또는 인간과의 대화에서 사용되는 단어를 보완, 제스처는 좀 더 섬세하고 조심스러움

나레이션 또는 대화를 나타내는 것을 one-hot encoding vecor={0,1}로 표현

**Intended Emotion**

제한된감정->NRC-VAD를 사용하여 텍스트와 관련된 이러한 범주적 감정을 VAD 공간으로 변환

VAD 공간은 감정을 모델링하기 위한 감성 컴퓨팅에서 잘 알려진 표현 valence(V), arousal(A), dominance(D)에 의해 확장된 3차원 공간의 점으로 감정을 매핑

Valence는 감정의 쾌적함의 척도이고(예: 행복 대 슬픔), arousal은 얼마나 활동적인지 또는 얼마나 활발한지를 나타내는 척도, dominance는 감정을 표현하는 대상이 자신의 행동에 대해 얼마나 통제하고 있다고 느끼는지를 나타내는 척도(예: 자부심 대 후회)

VAD를 나타내는 것을 3차원 정규화된 값으로 표현

#### 3.3.2 Attributes Depending on the Agent

Gender G, handedness H를 벡터로 표현

handedness 는 left hand dominant, right hand dominant를 {0,1}의 벡터로 표현

### 3.4 Using the Transformer Network

transformer network는 sequence-to-sequence modeling을 위해 encoder-decoder architecture을 따름

Attention 메커니즘은 key-value 쌍의 사전에서 가져온 값의 합으로 나타나며, 각 값은 key와 query에 관련성에 따라 결정됨

<img width="280" alt="image" src="https://user-images.githubusercontent.com/60170358/160271580-79d623c6-8f5c-4cd7-b011-f2e74e19fc9f.png">

자연어 텍스트의 문장을 받아들여 사전 훈련된 GloVe 모델을 사용하여 단어 임베딩으로 변환 -> transformer인코더를 사용하여 단어 임베딩을 latent representation표현으로 변환-> agent 속성을 이 latent representation에 추가하고 결합된 표현을 encoded feature로 변환

 transformer decoder은 이 encoded feature와 과거 제스처 기록을 수집하여 이후의 시간 단계에 대한 제스처를 예측

각 시간 단계에서, 시간 단계의 포즈 그래프에서 각각의 부모(이전)에 상대적인 모든 신체 관절의 회전 세트로 제스처를 표현



## 4 TRAINING THE TRANSFORMER-BASED NETWORK

angle loss, pose loss, affective loss를 사용하여 네트워크를 훈련

gesture sequences generated by our network와 the original motion-captured sequences 사이에서 계산

### 4.1 Angle Loss for Smooth Motions

Euler angle을 사용해서 원래의 angle과 예측한 angle의 loss를 계산

### 4.2 Pose Loss for Joint Trajectories

예측한 joint position과 원래의 joint의 차이를 squared norm으로 계산

### 4.3 Affective Loss for Emotive Gestures

gesture-based affective features of the ground-truth와 the predicted poses사이의 loss를 계산

Dominance가 높으면 upper body, spread arms, and upright head positions <-> Dominance가 낮으면 contracted upper body, arms close to the body, and collapsed head positions

arousal이 높으면 rapid arm swings and head movements <->arousal이 낮으면 subtle, slow movements

<img width="210" alt="image" src="https://user-images.githubusercontent.com/60170358/160550358-82b1597c-75ee-48ee-bbe0-31931cb1221b.png">



## 5 Results

### 5.1 Data for Training, Validation and Testing

- E as the VAD representation for one of “afraid”, “amused”, “angry”, “ashamed”, “disgusted”, “joyous”, “neutral”, “proud”, “relieved”, “sad”, or “surprised”
-  G is either female or male
-  H is either left or right

마지막에 EOS pose로 부드럽게 넘어가게 지정해서 모든 sequence들이 같은 길이를 갖도록 함

### 5.2 Training and Evaluation Routines

Adam optimizer, learning rate 0.001, 600 epochs, batch size 16

### 5.3 Comparative Performance

<img width="251" alt="image" src="https://user-images.githubusercontent.com/60170358/160553330-2251174f-a6f4-42db-9cc6-bf2e3df888a4.png">

우리의 network와 가장 비슷한 Yoon et al과 비교

time step에서 모든 joint에 대한 total pose error을 계산하고 모든 time step의 mean error을 계산

mean error를 가상 에이전트의 3D 경계 상자의 가장 긴 대각선의 평균 길이로 나누어 정규화된 평균 오차를 구함



### 5.4 Ablation Studies

<img width="225" alt="image" src="https://user-images.githubusercontent.com/60170358/160556153-ea173047-7772-4444-8a27-ed978bfc51bb.png">

angle loss가 없으면  없이 제스처가 심하게 흔들림

pose loss을 제거하면 제스처는 부드러워지지만 약간 흔들림

-> 이것은 포즈 손실이 또한 생성 과정에 어느 정도 견고함을 제공한다는 것을 보여줌

affective loss만 제거하면 네트워크는 다양한 제스처를 생성하여 유동적이고 그럴듯한 애니메이션을 만들 수 있지만 동작의 감정표현들은 의도한 감정과 일치하지 않음

### 5.5 Interfacing the VR Environment

제스처를 생성하는 동안 Virtual Agent의 물리적 속성을 결정하기 위해 성별과 핸드니스(handedness)를 사용

제스처 애니메이션을 위해 MPI 감정 신체 움직임 데이터베이스에 미리 정의된 남성 및 여성 골격 세트를 사용

생성된 애니메이션을 휴머노이드 메쉬에 맞추기 위해 Blender 2.7을 사용, Unreal 4.25를 이용하여 VR 환경을 준비

우리는 가상 에이전트를 장면 중앙의 의자에 완전히 초점을 맞추고 에이전트가 적절한 신체 제스처를 사용하여 한 줄씩 내레이션하는 스토리를 선택하거나, 에이전트가 텍스트 및 연관된 신체 제스처를 사용하여 응답하는 대화의 일부로 텍스트 줄을 보낼 수 있습니다. 

에이전트들을 위한 오디오에 얼굴 특징이나 감정을 추가하지 않음 왜냐하면 그것들은 감정표현의 지배적인 양식이고 제스처의 감정표현에 대한 공정한 평가를 어렵게 하기 때문-> 예를 들어, 의도한 감정이 행복하고 에이전트가 웃는 얼굴을 가지고 있다면, 관찰자는 높은 원자가 또는 흥분된 제스처에 호의적으로 반응할 가능성 높음

## 7 CONCLUSION

나레이션이나 대화의 의도된 행동 과제, 텍스트와 문맥에 기초한 의도된 감정, 그리고 그럴듯한 제스처를 생성하기 위해 가상 에이전트의 의도된 성별과 handedness를 고려

또한 생성된 제스처의 자연스러움과 감정 표현성을 평가하기 위해 웹 연구를 수행, 50명의 참가자의 총 600개의 응답을 바탕으로, 우리는 가상 에이전트의 제스처의 의도된 감정과 응답자가 인식한 감정 사이에 강력한 양의 상관 관계를 발견했으며, 최소 Pearson 계수는 0.77



## 8 LIMITATIONS AND FUTURE WORK

- 첫째, 완전한 텍스트 문장에서 제스처까지 매핑을 학습하도록 네트워크를 훈련-> 텍스트에서 제스처까지 보다 세분화된 phrase 수준의 매핑을 탐색하여 문장의 일부에 해당하는 제스처가 어떻게 조합되어 완전한 문장을 위한 제스처를 생성할 수 있는지에 대한 통찰력을 얻음으로써 이를 개선할 수 있음

- 둘째, 우리가 만든 제스처는 모든 문장을 손짓한 후에 EoS 포즈로 돌아가기 때문에 인접한 문장에 해당하는 제스처 사이의 연속성과 관련된 어떠한 정보도 이용할 수 없음 -> 이 접근 방식을 확장하는 간단한 방법은 현재 문장의 마지막 창을 다음 문장의 초기화로 사용하는 것

  그러나 제스처 간의 연속성에 대한 사실 정보가 없으면 전환 제스처를 훈련하거나 평가하는 것이 어려움

- 셋째, 우리는 텍스트와 관련된 범주형 감정 용어에 대해서만 VAD 표현을 고려 -> 네트워크 설계와 연구 참여자들이 인지하는 감정 평가가 단순해짐

  앞으로는 텍스트에서 단어의 VAD 표현과 연관된 범주형 감정 사이의 상관 관계를 살펴보고 VAD 표현과 피험자의 성별, 나이 및 민족성의 상호 관계를 연구하여 텍스트에서 보다 다양한 범위의 감정 제스처에 이르는 보다 정교한 지도를 구축할 계획

  감성 제스처 생성 알고리즘을 소셜 VR 시스템과 통합하여 사회 인식 대화 에이전트에 활용하고자 함

- 마지막으로, 우리는 텍스트 기반의 감정적인 제스처 생성만 고려하고 표정이나 표정이 풍부한 목소리 톤은 고려하지 않음

  실제 시나리오에서 표정과 목소리 톤은 감정을 전달하는 데 있어 지배적인 역할을 하는 경향이 있으며 사용자의 초점을 차지할 수 있음

  향후 작업의 일환으로, 우리의 작업을 다양한 감정에 대응하는 다양한 표정과 결합하고 목소리의 감정 톤을 변화시키는 것

  또한, 우리는 감정적인 몸짓, 표정, 목소리 톤과 같은 다른 양식들을 조합하는 것의 상대적 이점을 평가할 것임



## 생각했던점

이 논문은 단순히 여러 대화에 대한 일반대화+감정제스처 를 보여주는 것 같음-> 우리는 감정적 치료를 목적으로 하는것이라 차별성 있을수도?

Transformer로 Encoded feature을 만들고 그 feature에 해당하는 gesture을 mapping해주는것을 활용 이때 이전 gesture 정보까지 담음 -> 우리 프로젝트에 활용해볼점 **

얼굴 표정은 고려사항, 이 논문에서는 안하는것이 좋다고함, 여러가지 평가요소로 사용할 수 있을듯

VAD 감정표현 방식 좋은듯

궁금한점) encoded emotion-gesture 이렇게 mapping한다고 하면 angry-gesture가 있는것? 그래서 그 gesture와 예측한 gesture사이의 loss를 측정하는 건가?

